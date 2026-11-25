#ifndef MAP_H
#define MAP_H

// #define MAP_WIDTH  64
// #define MAP_HEIGHT 36

#define MAP_WIDTH  36
#define MAP_HEIGHT 18

// #define MAP_WIDTH  18
// #define MAP_HEIGHT 9

#define TILE_SIZE  20
#define MAX_ROOMS 200

#include "room.h"

#include "helpers.h"

typedef enum TileType {
    TILE_TYPE_WALL,
    TILE_TYPE_FLOOR,
    TILE_TYPE_STATIC, // block player and enemies but not field of view!
    TILE_TYPE_UNDEFINED,
}TileType;

typedef enum TileLayer{
    TILE_LAYER_GROUND,
    TILE_LAYER_DECORATION,
    TILE_LAYER_COUNT,
}TileLayer;

typedef struct Tile
{
    TileType type;
    bool visible;
    bool explored;

    int visual_index;
    int visual_decoration_index;
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

    Vector2i can_respawn_point[MAP_HEIGHT * MAP_WIDTH];
    int can_respawn_point_count;
}Map;

void map_init(Map *self);
void map_init_cave(Map *self);
void map_init_village(Map *self);
bool map_is_inside(const Map *self, int x, int y);
bool map_is_visible(const Map *self, int x, int y);
bool map_is_visible_edge(const Map *self, int x, int y);
void map_compute_distance_from_goal(Map *self, int goal_x, int goal_y);
void map_print(Map *self);
bool map_is_floor(const Map *self, int x, int y);
void map_update_respawn_points(Map *self);
Vector2i map_get_first_respawnable_point(Map *self);

#endif // MAP_H
