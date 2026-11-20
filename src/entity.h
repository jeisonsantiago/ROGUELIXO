#ifndef ENTITY_H
#define ENTITY_H

#include "player.h"
#include "item.h"
#include "inventory.h"
#include "entity_status.h"
#include "trait.h"
#include "monster.h"
#include "npc.h"

#define BASE_DAMAGE 1
#define BASE_DEFENSE 0

typedef struct Map Map;
typedef struct GamePlay GamePlay;

typedef enum EntityType{
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_MONSTER,
    ENTITY_TYPE_NPC,
}EntityType;

typedef enum EntityState{
    STATE_IDLE,
    STATE_ATTACKING,
}EntityState;

typedef struct Entity{
    int x;
    int y;
    int hp;
    int max_hp;
    bool alive;

    EntityState state;

    float visual_x;
    float visual_y;

    float start_x;
    float start_y;
    float target_x;
    float target_y;

    bool is_moving;
    float timer_moving;

    // entity attack vars
    float timer_attacking;
    float timer_attacking_counter;
    Entity *target;

    float animation_sin_wave;

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
        NPC npc;
    }data;

}Entity;

void entity_init(Entity *self, EntityType type);
void entity_set_position(Entity *self, int x, int y);
bool entity_try_move(Entity *self,int dx, int dy, const Map *map);
// bool entity_try_move_smooth(Entity *self,int dx, int dy, const Map *map, float delta_time);
bool entity_is_alive(Entity *self);
float entity_distance(Entity *self, Entity *other);
void entity_update(Entity *self, float delta_time);
void entity_move_and_slide(Entity *self,float delta_time);
void entity_set_sprite(Entity *self,int sprite_index);


// extra
void entity_start_attacking(Entity *self, Entity *target);
bool entity_attack(Entity *attack, Entity *def);
bool entity_step_if_free(Entity *self, GamePlay *gameplay, int dx, int dy);

// inventory
int entity_add_to_inventory(Entity *self, Item item);
void entity_quaff(Entity *self, int slot);
void entity_equip(Entity *self, int slot); // can be weapon or amor (maybe more stuff in the future)
void entity_unequip(Entity *self, int slot);

#endif // ENTITY_H
