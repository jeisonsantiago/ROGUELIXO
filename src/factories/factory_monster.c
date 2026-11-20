#include "factory_monster.h"

#include "entity.h"
#include "dungeon.h"

void factory_monster(Dungeon *dungeon, int x, int y)
{
    int monster_index = dungeon->monster_count;
    // TraceLog(LOG_INFO, "monster created index: %i",monster_index);
    Entity *entity = &dungeon->monster[monster_index];
    dungeon->monster_count++;
    entity_init(entity,ENTITY_TYPE_MONSTER);

    entity_set_position(entity,x,y);

    strcpy(entity->name,"monster");
    entity->max_hp = 10;
    entity->hp = entity->max_hp;
    entity->damage = 1;
    entity->alive = true;

    entity->trait.type = TRAIT_VENOMOUS;
    entity->trait.turns = 2;
    entity->trait.chance = 25; // 25% chance of proc

}

