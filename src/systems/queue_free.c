#include "queue_free.h"

#include "dungeon.h"

void queue_free_system(Dungeon *dungeon){
    for (int i = 0; i < dungeon->monster_count; ++i) {

        Entity *entity = &dungeon->monster[i];

        if(entity->hp <= 0){
            dungeon_remove_monster_by_index(dungeon,i);
        }
    }
}