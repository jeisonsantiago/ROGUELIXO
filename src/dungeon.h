#ifndef DUNGEON_H
#define DUNGEON_H

#include "map.h"
#include "item.h"
#include "entity.h"
#include "helpers.h"

#define MAX_ITEMS 200 // max items per dungeon
#define MAX_MONSTERS 20

typedef enum DungeonType{
    DUNGEON_TYPE_UNDEFINED,
    DUNGEON_TYPE_ROOMS,
    DUNGEON_TYPE_CAVE,
    DUNGEON_TYPE_VILLAGE,
}DungeonType;

typedef struct Dungeon{
    Map map;
    bool generated;

    int tile_index_wall;
    int tile_index_floor;

    Item items[MAX_ITEMS];
    int item_count;

    Entity monster[MAX_MONSTERS];
    int monster_count;

    int player_start_x;
    int player_start_y;

    // to go down
    int stairs_down_x;
    int stairs_down_y;

    // to go up
    int stairs_up_x;
    int stairs_up_y;
    bool stairs_up_active;

    DungeonType type;
}Dungeon;

// void dungeon_init(Dungeon *self);

void dungeon_set(Dungeon *self,DungeonType type);
void dungeon_init(Dungeon *self, int player_x, int player_y, DungeonType type);
void dungeon_remove_item(Dungeon *self, int x, int y);
void dungeon_remove_monster(Dungeon *self, int x, int y);

void dungeon_remove_monster_by_index(Dungeon *self, int index);
Vector2i dungeon_random_position(Dungeon *self, int not_x, int not_y, int room);
Vector2i dungeon_random_position_room(Dungeon *self, int room);
Vector2i dungeon_random_position_cave(Dungeon *self);
// void dungeon_populate(Dungeon *self, int player_x, int player_y, DungeonType type);
bool dungeon_has_stairs_up(Dungeon *self);
bool dungeon_has_stairs_down(Dungeon *self);

#endif // DUNGEON_H
