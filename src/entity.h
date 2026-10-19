#ifndef ENTITY_H
#define ENTITY_H

#include "player.h"
#include "item.h"
#include "inventory.h"
#include "entity_status.h"
#include "trait.h"
#include "monster.h"

#define BASE_DAMAGE 1
#define BASE_DEFENSE 0

typedef struct Map Map;
typedef struct GamePlay GamePlay;

typedef enum EntityType{
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_MONSTER,
}EntityType;

typedef struct Entity{
    int x;
    int y;
    int hp;
    int max_hp;
    bool alive;

    int damage;
    int defence;

    // status
    int strength_bonus;

    bool has_weapon;
    bool has_armour;

    // equiped (IT is not in the inventory)
    Item weapon;
    Item armour;

    Trait trait;
    Status status[STATUS_COUNT];

    char name[STR_LENGHT];
    EntityType entity_type;

    Inventory inventory;

    int texture_asset_index;
    int texture_sprite_index;

    union{
        Player player;
        Monster monster;
    }data;
}Entity;

void entity_init(Entity *self, EntityType type);
bool entity_try_move(Entity *self,int dx, int dy, const Map *map);
bool entity_is_alive(Entity *self);
float entity_distance(Entity *self, Entity *other);
void entity_update(Entity *self);

// extra
void entity_attack(Entity *attack, Entity *def);
bool entity_step_if_free(Entity *self, GamePlay *gameplay, int dx, int dy);

// inventory
int entity_add_to_inventory(Entity *self, Item item);
void entity_quaff(Entity *self, int slot);
void entity_equip(Entity *self, int slot); // can be weapon or amor (maybe more stuff in the future)
void entity_unequip(Entity *self, int slot);

#endif // ENTITY_H
