#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH  64
#define MAP_HEIGHT 36
#define TILE_SIZE  20
#define MAX_ROOMS 200

#include "room.h"

typedef enum TileType {
    TILE_TYPE_WALL,
    TILE_TYPE_FLOOR,
    TILE_TYPE_CONTOUR,
}TileType;

typedef struct Tile
{
    TileType type;
    bool visible;
    bool explored;
}Tile;

typedef struct BSPNode{
    int x;
    int y;
    int w;
    int h;

    int node_left;
    int node_right;

    Room room;
    bool has_room;
}BSPNode;

typedef struct Map{
    Tile tiles[MAP_HEIGHT][MAP_WIDTH];
    int dist[MAP_HEIGHT][MAP_WIDTH];
    Room rooms[MAX_ROOMS];
    int rooms_count;

    BSPNode nodes[MAX_ROOMS];
    int nodes_count;
}Map;

void map_init(Map *self);
bool map_is_inside(const Map *self, int x, int y);
bool map_is_visible_edge(const Map *self, int x, int y);
void map_compute_distance_from_goal(Map *self, int goal_x, int goal_y);
void map_print(Map *self);

#endif // MAP_H
