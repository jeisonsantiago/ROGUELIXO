#include "inventory.h"
#include "shared_context.h"
#include "gameplay.h"

// // clear inventory

static void inventory_check_eqquiped(Inventory *self){

}

void inventory_init(Inventory *self)
{
    self->selected_slot = 0;
    self->inventory_count = 0;
    memset(self->inventory,0,sizeof self->inventory);


    strcpy(self->description,"");
}

int inventory_add_item(Inventory *self, Item item)
{
    if(self->inventory_count >= MAX_INVENTORY_SLOTS){
        return -1; // inventory full capacity
    }

    self->inventory[self->inventory_count++] = (InventorySlot){.item = item, .occupied = true};
    // int index = self->inventory_count;

    // self->inventory[index].item = item;
    // self->inventory[index].occupied = true;
    // self->inventory_count = index + 1;
    // TraceLog(LOG_INFO,"ITEM ADDED");
    return 1;
}

bool inventory_remove(Inventory *self, int slot)
{
    self->inventory[slot].occupied = false;
    return false;
}

void inventory_render(Inventory *self, Camera2D *camera)
{
    int w = SCREEN_WIDTH;
    int h = SCREEN_HEIGHT;

    // background
    DrawRectangle(0,0,w,h,Fade(BLACK,0.4f));

    // center
    int inventory_w = 250;
    int inventory_h = 200;

    NPatchInfo ninePatchInfo1 = { (Rectangle){ 0.0f, 0.0f, 64.0f, 64.0f }, 12, 40, 12, 12, NPATCH_NINE_PATCH };

    Vector2 s2w = GetScreenToWorld2D((Vector2){w*0.5f,h*0.5f},*camera); // center
    Rectangle dest = (Rectangle){s2w.x - (inventory_w * 0.5f),s2w.y - (inventory_h * 0.5f),inventory_w, inventory_h};
    Rectangle ref = dest;
    DrawRectangleRec(dest,DARKGRAY);
    DrawRectangleLinesEx(dest,1,GRAY);

    dest.x += 7;
    dest.y += 32;
    // draw 20 slots
    int rect_counter = 0;
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 10; ++x) {
            Rectangle rect_slot = (Rectangle){dest.x+(x*24),dest.y+(y*20),20,20};
            self->rect_slots[rect_counter] = rect_slot;
            rect_counter++;
            DrawRectangleLinesEx(rect_slot,1,GRAY);
        }
        dest.y += 6;
    }

    // WEAPON SLOT
    self->rect_weapon_slot = (Rectangle){dest.x,dest.y+(50),20,20};
    DrawRectangleLinesEx(self->rect_weapon_slot,1,GRAY);
    DrawText("Weapon",self->rect_weapon_slot.x+25, self->rect_weapon_slot.y+5,10,WHITE);

    // ARMOR SLOT
    self->rect_armor_slot = (Rectangle){dest.x,dest.y+(76),20,20};
    DrawRectangleLinesEx(self->rect_armor_slot,1,GRAY);
    DrawText("Armor",self->rect_armor_slot.x+25, self->rect_armor_slot.y+5,10,WHITE);

    AssetManager *asset_manager = &shared_context()->asset_manager;
    // TextureAsset *ta = &asset_manager->interface;
    // Rectangle src_floor = asset_manager_get_texture_rect_by_index(16*7,ta);

    // render selected slot
    DrawRectangleLinesEx(self->rect_slots[self->selected_slot],1,YELLOW);

    // draw text
    int text_lenght = MeasureText("Inventory",16);
    DrawText("Inventory",ref.x+(ref.width*0.5f)-(text_lenght*0.5f), ref.y+6,16,WHITE);

    char *text = "[E] to (use/equip/unequip) item \n[ESC] or [I] to exit inventory.";
    text_lenght = MeasureText(text,10);
    DrawText(text,ref.x+(ref.width*0.5f)-(text_lenght*0.5f), ref.y+160,10,WHITE);

    Rectangle description  = {
        self->rect_weapon_slot.x+75, self->rect_weapon_slot.y,162,45
    };

    DrawRectangleLinesEx(description,1,GRAY);
    if(self->inventory_count >= 0){
        // item info
        DrawText(self->description,self->rect_weapon_slot.x+80, self->rect_weapon_slot.y+5, 2,WHITE);
    }

    // render items in the inventory
    if(self->inventory_count >= 0){
        for (int i = 0; i < self->inventory_count; ++i) {


            InventorySlot *slot = &self->inventory[i];

            if(!slot->occupied) continue;

            Item *item = &slot->item;

            TextureAsset *ta = &asset_manager->items;

            Rectangle src = asset_manager_get_texture_rect_by_index(item->sprite_index,ta);

            Rectangle dest = self->rect_slots[i];

            DrawTexturePro(ta->texture,src,dest,(Vector2){0,0},0,WHITE);

            if(item->equipped){

                DrawRectangleRec(dest,Fade(RED,0.5f));

                switch (item->type) {
                case ITEM_TYPE_WEAPON:{
                    DrawTexturePro(ta->texture,src,self->rect_weapon_slot,(Vector2){0,0},0,WHITE);
                    break;
                }
                case ITEM_TYPE_ARMOR:{
                    DrawTexturePro(ta->texture,src,self->rect_armor_slot,(Vector2){0,0},0,WHITE);
                    break;
                }
                default:
                    break;
                }
            }
        }
    }
}

int inventory_update(Inventory *self, GamePlay *gameplay){


    if(IsKeyPressed(KEY_ESCAPE)){
        gameplay->inventory = false;
    }

    int slot = self->selected_slot;
    strcpy(self->description,self->inventory[self->selected_slot].item.description);

    if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_LEFT]){
        slot--;
        if(slot >= 0 && slot  < MAX_INVENTORY_SLOTS) {
            self->selected_slot--;
        }
    }
    if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_RIGHT]){
        slot++;
        if(slot >= 0 && slot  < MAX_INVENTORY_SLOTS) {
            self->selected_slot++;
        }
    }

    // up and down
    if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_UP]){
        slot -= 10;
        if(slot >= 0 && slot  < MAX_INVENTORY_SLOTS) {
            self->selected_slot -= 10;
        }
    }
    if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_DOWN]){
        slot += 10;
        if(slot >= 0 && slot  < MAX_INVENTORY_SLOTS) {
            self->selected_slot += 10;
        }
    }

    // check if it will use it
    if(shared_context()->input_state.time_out_actions_pressed[ACTION_USE]){
        if(gameplay->player.inventory.inventory[self->selected_slot].occupied){

            // check if it's consumable or equip
            Item *item = &self->inventory[self->selected_slot].item;
            switch (item->type) {
            case ITEM_TYPE_POTION:{

                if(item->data.potion.type == POTION_HEALING){
                    entity_quaff(&gameplay->player,self->selected_slot);
                    inventory_remove(&gameplay->player.inventory,self->selected_slot);
                }
                break;
            }
            case ITEM_TYPE_WEAPON:{

                // if equiped swap
                if(gameplay->player.has_weapon){
                    entity_unequip(&gameplay->player,gameplay->player.weapon);
                    entity_equip(&gameplay->player,self->selected_slot);
                }
                break;
            }
            case ITEM_TYPE_ARMOR:{ // swap?
                if(gameplay->player.has_armour){
                    entity_unequip(&gameplay->player,gameplay->player.armour);
                    entity_equip(&gameplay->player,self->selected_slot);
                }
                break;
            }
            default:
                break;
            }

            gameplay->inventory = false;
            TraceLog(LOG_INFO,"CARRY ON WITH THE TURN!");
            return 1;
        }
    }

    return 0;

}