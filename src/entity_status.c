#include "entity_status.h"
#include "entity.h"
#include "helpers.h"

void status_add(Entity *entity, Status status, int turns)
{
    if(turns > entity->status[status]){
        entity->status[status] = turns;
    }
}

bool status_has(Entity *entity, Status status)
{
    return entity->status[status] > 0;
}


void status_update(Entity *entity)
{
    if(entity->status[STATUS_POISONED] > 0){
        entity->hp -= 1;
        entity->status[STATUS_POISONED]--;
        TraceLog(LOG_INFO,"%s is wracked by poison. (hp %d)", entity->name, entity->hp > 0 ? entity->hp : 0);
        if (entity->hp <= 0) {
            entity->hp = 0; entity->alive = false; TraceLog(LOG_INFO,"%s succumbs to poison.", entity->name);
        }
    }

    if(entity->status[STATUS_REGEN] > 0){
        if(entity->hp < entity->max_hp){
            entity->hp += 1;
            entity->status[STATUS_REGEN]--;
        }
    }
}


void status_apply(Entity *attack, Entity *defence){
    if(attack->trait.type == TRAIT_VENOMOUS){
        if(chance(attack->trait.chance)){
            status_add(defence,STATUS_POISONED,attack->trait.turns);
        }
    }
}
Color status_color(Entity *entity)
{
    Color color = WHITE;

    for (int i = 0; i < STATUS_COUNT; ++i) {
        if(entity->status[i] > 0){
            if(i == STATUS_POISONED){
                return Fade(GREEN,0.5f);
            }
        }
    }

    return color;
}
