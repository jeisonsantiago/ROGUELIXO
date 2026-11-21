#include "entity.h"
#include "map.h"
#include "gameplay.h"
#include "entity_status.h"

void entity_init(Entity *self, EntityType type){

    memset(self,0,sizeof(&self));

    self->x = 0;
    self->y = 0;    
    self->target_x = 0;
    self->target_y = 0;
    self->start_x = 0;
    self->start_y = 0;
    self->visual_x = 0.0f;
    self->visual_y = 0.0f;
    self->animation_sin_wave = 0.0f;
    self->state = STATE_IDLE;
    self->timer_moving = 0.0f;
    self->max_hp = 30;
    self->hp = self->max_hp;
    self->entity_type = type;
    self->alive = true;
    self->damage = 1;
    self->defence = 0;
    strcpy(self->name,"");
    self->strength_bonus = 0;
    self->has_armour = false;
    self->has_weapon = false;
    self->weapon = (Item){.type = ITEM_TYPE_UNDEFINED}; // empty
    self->armour = (Item){.type = ITEM_TYPE_UNDEFINED}; // empty
    self->can_move = true;

    inventory_init(&self->inventory);

    switch (type) {
    case ENTITY_TYPE_PLAYER:{
        player_entity_setup(self);
        self->texture_asset_index = 2;
        self->texture_sprite_index = 0;    
        break;
    }
    case ENTITY_TYPE_MONSTER:{
        monster_entity_setup(self);
        self->texture_asset_index = 2;
        self->texture_sprite_index = 4;    
        break;
    }
    case ENTITY_TYPE_NPC:{
        monster_entity_setup(self);
        self->texture_asset_index = 2;
        self->texture_sprite_index = 0;

        // npc_entity_setup(self,"");

        break;

        // self.
    }
    default:
        break;
    }

    for (int i = 0; i < STATUS_COUNT; ++i) {
        self->status[i] = 0;
    }

    // init
    self->timer_attacking = 0.3f;
}

bool entity_is_alive(Entity *self) {
    return self->alive && self->hp > 0;
}

float entity_distance(Entity *self, Entity *other){
    return Vector2Distance((Vector2){self->x,self->y},(Vector2){other->x,other->y});
}

void entity_start_attacking(Entity *self, Entity *target){

    if(target->entity_type == ENTITY_TYPE_NPC ) return;

    self->animation_sin_wave = 0.0f;
    TraceLog(LOG_INFO,"start attacking");
    self->state = STATE_ATTACKING;
    self->timer_attacking_counter = self->timer_attacking;
    self->target = target;
}

bool entity_attack(Entity *attack, Entity *def)
{
    // attack->animation_sin_wave = 0.0f; // jeison santiago
    attack->timer_attacking_counter = 0.0f;

    int damage = attack->damage - def->defence;
    if(damage < 1) damage = 1; // clean hit always hit

    // check for status
    status_apply(attack,def);

    def->hp -= damage;
    TraceLog(
                LOG_INFO,
                "%s hits %s for %d. (%s: %d hp)",
                attack->name,
                def->name,
                attack->damage,
                def->name, def->hp > 0 ? def->hp : 0);

    if(def->hp <= 0){
        def->alive = false;
        TraceLog(LOG_INFO, "%s dies.", def->name);
        // remove entity
    }

    return true;
}

bool entity_step_if_free(Entity *self, GamePlay *gameplay, int dx, int dy)
{
    if(dx == 0 && dy == 0) return false;

    int nx = self->x + dx;
    int ny = self->y + dy;

    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    if(!map_is_inside(&dungeon->map,nx, ny)) return false;
    if(dungeon->map.tiles[ny][nx].type != TILE_TYPE_FLOOR) return false;
    if(nx == gameplay->player.x && ny == gameplay->player.y) return false; // never stack them
    if(monster_at(gameplay, nx, ny)) return false; // cant stack monster as well

    self->x = nx;
    self->x = ny;

    return true;
}

void entity_quaff(Entity *self, int slot)
{
    if (slot < 0 || slot >= self->inventory.inventory_count) return;

    Item *item = &self->inventory.inventory[slot].item;
    ItemType item_type = item->type;

    switch (item_type) {
    case ITEM_TYPE_POTION:{
        if(item->data.potion.type == POTION_HEALING && self->hp < self->max_hp) {
            int amount = item->data.potion.amount;
            self->hp += amount;
            // TraceLog(LOG_INFO,"Player healed by %i amount", amount);
            TraceLog(LOG_INFO,"%s healed %i hp amount", self->name,amount);
        }
        break;
    }
    default:
        break;
    }
}

void entity_equip(Entity *self, int slot)
{
    if (slot < 0 || slot >= self->inventory.inventory_count) return;

    Item *item = &self->inventory.inventory[slot].item;

    switch (item->type) {
    case ITEM_TYPE_WEAPON:{
        // set inventory to equipped and copy to entity
        self->has_weapon = true;
        item->equipped = true;
        self->weapon = *item; // just copy...no fidling with pointers please
        TraceLog(LOG_INFO,"%s equiped", self->name);
        break;
    }
    case ITEM_TYPE_ARMOR:{
        // set inventory to equipped and copy to entity
        self->has_armour = true;
        item->equipped = true;
        self->weapon = *item; // just copy...no fidling with pointers please
        TraceLog(LOG_INFO,"%s equiped", self->name);
        break;
    }
    default:
        break;
    }
}

void entity_unequip(Entity *self, int slot)
{
    if (slot < 0 || slot >= self->inventory.inventory_count) return;

    Item *item = &self->inventory.inventory[slot].item;

    switch (item->type) {
    case ITEM_TYPE_WEAPON:{
        // set inventory to equipped and copy to entity
        item->equipped = false;
        self->weapon = (Item){0};
        TraceLog(LOG_INFO,"%s unequiped", self->name);
        break;
    }
    case ITEM_TYPE_ARMOR:{
        item->equipped = false;
        self->weapon = (Item){0};
        TraceLog(LOG_INFO,"%s unequiped", self->name);
        break;
    }
    default:
        break;
    }
}

void entity_update(Entity *self, float delta_time)
{
    if(self->state == STATE_IDLE){
        self->animation_sin_wave = 0.0f;
    }

    // compute stats (weapons, defence etc)
    self->damage = BASE_DAMAGE + self->strength_bonus + ((self->has_weapon) ? self->weapon.data.weapon.amount:0);
    self->defence = BASE_DEFENSE + ((self->has_armour) ? self->armour.data.armor.amount:0);

    if(self->state != STATE_ATTACKING) return;


    self->timer_attacking_counter -= delta_time;

    // if(self->entity_type == ENTITY_TYPE_MONSTER){
    //     TraceLog(LOG_INFO,"att counter: %f",self->timer_attacking_counter);
    // }

    // float u = 0.5f - t;   /* when t goes 0.5→0, u goes 0→0.5 */
    // float wave = sinf(u * 2.0f * 3.14159f);

    if(self->state == STATE_ATTACKING){
        float u = (self->timer_attacking - self->timer_attacking_counter) / self->timer_attacking;

        float ease = u * u * u * (u * (u * 6.0f - 15.0f) + 10.0f);

        // self->animation_sin_wave = ease;
        self->animation_sin_wave = sinf(ease * PI);
        // TraceLog(LOG_INFO,"sin wave: %f",self->animation_sin_wave);
    }

    if(self->timer_attacking_counter <= 0 && self->target != NULL){
        entity_attack(self,self->target);
        self->state = STATE_IDLE;
        self->target = NULL;
    }
}

bool entity_try_move(Entity *self,int dx, int dy, const Map *map){


    if(!self->can_move) return false;

    if(self->state == STATE_ATTACKING) return false;

    if(self->is_moving) return false;

    int nx = self->x + dx;
    int ny = self->y + dy;

    if(!map_is_inside(map,nx, ny)) return false;

    if(map->tiles[ny][nx].type != TILE_TYPE_FLOOR) return false;

    self->start_x = self->visual_x;
    self->start_y = self->visual_y;

    self->x = nx;
    self->y = ny;

    self->target_x = self->x;
    self->target_y = self->y;

    self->is_moving = true;

    self->timer_moving = 0.0f;

    return true;
}

void entity_move_and_slide(Entity *self,float delta_time){
    if(!self->is_moving) return;

    self->timer_moving += delta_time / 0.15f; // 0.15 seconds per tile

    if(self->timer_moving >= 1.0f){
        self->visual_x = self->target_x;
        self->visual_y = self->target_y;
        self->is_moving = false;
    }else{
        self->visual_x = self->start_x + (self->target_x - self->start_x) * self->timer_moving;
        self->visual_y = self->start_y + (self->target_y - self->start_y) * self->timer_moving;
    }
}

void entity_set_position(Entity *self, int x, int y)
{
    self->x = x;
    self->y = y;

    self->target_x = x;
    self->target_y = y;

    self->start_x = x;
    self->start_y = y;

    self->visual_x = x;
    self->visual_y = y;
}

void entity_set_sprite(Entity *self, int sprite_index)
{
    self->texture_sprite_index = sprite_index;
}
