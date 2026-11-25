#ifndef PLAYER_H
#define PLAYER_H

// #include "helpers.h"
// #include "entity.h"


typedef struct Entity Entity;
typedef struct Map Map;
typedef struct GamePlay GamePlay;

typedef struct Player{
    // Entity entity;

    // Vector2 edges[360];
    // int edges_count;
    int fov_distance;
}Player;

// void player_init(Entity *self);
void player_entity_setup(Entity *entity);
void player_calculate_fov(GamePlay *gameplay);
void player_pickup_here(GamePlay *gameplay);

// void render_player(Player *self);
void player_show_map(GamePlay *gameplay);

#endif // PLAYER_H
