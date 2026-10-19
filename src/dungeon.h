#ifndef DUNGEON_H
#define DUNGEON_H

#include "map.h"
#include "item.h"
#include "entity.h"

#define MAX_ITEMS 200 // max items per dungeon
#define MAX_MONSTERS 20


typedef struct Dungeon{
    Map map;

    int tile_index_wall;
    int tile_index_floor;

    Item items[MAX_ITEMS];
    int item_count;

    Entity monster[MAX_MONSTERS];
    int monster_count;

    int stairs_x;
    int stairs_y;
}Dungeon;

// void dungeon_init(Dungeon *self);

void dungeon_init(Dungeon *self, int player_x, int player_y);
void dungeon_remove_item(Dungeon *self, int x, int y);

#endif // DUNGEON_H
