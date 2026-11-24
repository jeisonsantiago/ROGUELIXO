#include "factory_set_levels.h"

#include "gameplay.h"
#include "entity.h"
#include "factory_monster.h"

void factory_set_levels(GamePlay *gameplay)
{
    // if(!gameplay) return;

    // Dungeon *dungeon = &gameplay->dungeon[0]; // pointer to the first dungeon
    // dungeon_set(dungeon,DUNGEON_TYPE_ROOMS);
    // dungeon_populate(dungeon,0,0,1,DUNGEON_TYPE_ROOMS);
    // entity_set_position(&gameplay->player,dungeon->player_start_x, dungeon->player_start_y);

    // for (int m = 0; m < 1; ++m) {
    //     Vector2i monster_pos = dungeon_random_position_room(dungeon,0);
    //     factory_monster(dungeon,monster_pos.x, monster_pos.y);
    //     TraceLog(LOG_INFO, "monster created: %i %i",monster_pos.x, monster_pos.y);
    // }

    // return;
    //-----------------------------------------------------------------------

    Dungeon *dungeon = &gameplay->dungeon[0]; // pointer to the first dungeon
    dungeon_set(dungeon,DUNGEON_TYPE_VILLAGE);
    // SET LEVEL 0 (VILAGE)
    dungeon_init(dungeon,0,0,DUNGEON_TYPE_VILLAGE);
    entity_set_position(&gameplay->player,dungeon->player_start_x, dungeon->player_start_y);
    dungeon->generated = true;

    //-----------------------------------------------------------------------

    // SET LEVEL 1 (FIRST FLOOR)
    // gameplay->current_dungeon++;
    dungeon = &gameplay->dungeon[1];
    dungeon_set(dungeon,DUNGEON_TYPE_ROOMS);

    //-----------------------------------------------------------------------

    dungeon = &gameplay->dungeon[2];
    dungeon_set(dungeon,DUNGEON_TYPE_ROOMS);

}
