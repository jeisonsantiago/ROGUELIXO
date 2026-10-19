#include "entity.h"
#include "map.h"
#include "gameplay.h"
#include "entity_status.h"

void entity_init(Entity *self, EntityType type){
    self->x = 0;
    self->y = 0;
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

    for (int i = 0; i < STATUS_COUNT; ++i) {
        self->status[i] = 0;
    }

    switch (type) {
    case ENTITY_TYPE_PLAYER:{
        player_entity_setup(self);
        self->texture_asset_index = 2;
        self->texture_sprite_index = 0;
    }
        break;
    case ENTITY_TYPE_MONSTER:{
        monster_entity_setup(self);
        self->texture_asset_index = 2;
        self->texture_sprite_index = 4;
    }
        break;
    default:
        break;
    }
}

bool entity_try_move(Entity *self,int dx, int dy, const Map *map){
    int nx = self->x + dx;
    int ny = self->y + dy;

    if(!map_is_inside(map,nx, ny)) return false;

    if(map->tiles[ny][nx].type != TILE_TYPE_FLOOR) return false;

    self->x = nx;
    self->y = ny;

    return true;
}

bool entity_is_alive(Entity *self) {
    return self->alive && self->hp > 0;
};

float entity_distance(Entity *self, Entity *other){
    return Vector2Distance((Vector2){self->x,self->y},(Vector2){other->x,other->y});
}
void entity_attack(Entity *attack, Entity *def)
{
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
    }
}

bool entity_step_if_free(Entity *self, GamePlay *gameplay, int dx, int dy)
{
    if(dx == 0 && dy == 0) return false;

    int nx = self->x + dx;
    int ny = self->y + dy;

    if(!map_is_inside(&gameplay->dungeon.map,nx, ny)) return false;
    if(gameplay->dungeon.map.tiles[ny][nx].type != TILE_TYPE_FLOOR) return false;
    if(nx == gameplay->player.x && ny == gameplay->player.y) return false; // never stack them
    if(monster_at(gameplay, nx, ny)) return false; // cant stack monster as well

    self->x = nx;
    self->x = ny;

    return true;
}

void entity_quaff(Entity *self, int slot)
{
    if (slot < 0 || slot >= self->inventory.inventory_count) return;

    ItemType item_type = self->inventory.inventory[slot].item.type;

    switch (item_type) {
    case ITEM_TYPE_POTION_HEALING:{
        if(self->hp < self->max_hp) {
            int amount = self->inventory.inventory[slot].item.amount;
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
        item->equipped = true;
        self->weapon = *item; // just copy...no fidling with pointers please
        TraceLog(LOG_INFO,"%s equiped", self->name);
        break;
    }
    case ITEM_TYPE_ARMOR:{
        // set inventory to equipped and copy to entity
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

void entity_update(Entity *self)
{
    // compute stats (weapons, defence etc)
    self->damage = BASE_DAMAGE + self->strength_bonus + ((self->has_weapon) ? self->weapon.amount:0);
    self->defence = BASE_DEFENSE + ((self->has_armour) ? self->armour.amount:0);
}


