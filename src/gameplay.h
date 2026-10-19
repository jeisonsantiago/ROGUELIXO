#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "dungeon.h"
#include "entity.h"



typedef enum GameState{
    GAME_STATE_PLAYING,
    // GAME_STATE_INVENTORY,
    GAME_STATE_DEAD,
}GameState;

typedef struct GamePlay{

    GameState game_state;

    Dungeon dungeon;
    Entity player;
    bool dead;
    bool dirty;
    Camera2D camera;

    bool inventory;
    // Entity monster;


}GamePlay;

void gameplay_init(GamePlay *self);
void gameplay_update(GamePlay *self, float delta_time);
void gameplay_render(GamePlay *self, float delta_time);

#endif // GAMEPLAY_H
