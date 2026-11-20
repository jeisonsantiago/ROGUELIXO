#include "factory_set_levels.h"

#include "gameplay.h"
#include "entity.h"

void factory_set_levels(GamePlay *gameplay)
{
    // if(!gameplay) return;

    Dungeon *dungeon = &gameplay->dungeon[0]; // pointer to the first dungeon
    dungeon_set(dungeon,DUNGEON_TYPE_VILLAGE);
    // SET LEVEL 0 (VILAGE)
    dungeon_populate(dungeon,0,0,1,DUNGEON_TYPE_VILLAGE);
    entity_set_position(&gameplay->player,dungeon->player_start_x, dungeon->player_start_y);

    //-----------------------------------------------------------------------

    // SET LEVEL 1 (FIRST FLOOR)
    // gameplay->current_dungeon++;
    dungeon = &gameplay->dungeon[1];
    dungeon_set(dungeon,DUNGEON_TYPE_ROOMS);
    // dungeon_populate(dungeon,1,1,gameplay->current_dungeon, DUNGEON_TYPE_ROOMS);

    dungeon = &gameplay->dungeon[2];
    dungeon_set(dungeon,DUNGEON_TYPE_ROOMS);

    // gameplay->dungeon_count = 2;
    // dungeon = &gameplay->dungeon[3];
    // dungeon_set(dungeon,DUNGEON_TYPE_ROOMS);



    // // // compute map distance
    // map_compute_distance_from_goal(&dungeon->map,gameplay->player.x, gameplay->player.y);

    // gameplay->camera.target.x = gameplay->player.x * TILE_SIZE;
    // gameplay->camera.target.y = gameplay->player.y * TILE_SIZE;

    // if(gameplay->current_dungeon == (DUNGEON_COUNT-1)){
    //     dungeon->stairs_x = 0;
    //     dungeon->stairs_y = 0;
    // }
}
