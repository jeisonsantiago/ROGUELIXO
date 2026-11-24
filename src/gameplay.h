#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "dungeon.h"
#include "entity.h"
#include "ranged_mode.h"
#include "screen.h"
#include "render_game_ui.h"

#define DUNGEON_COUNT 10

typedef enum GameScreen{
    SCREEN_PLAYING,
    SCREEN_DEAD,
    SCREEN_MENU,
}GameScreen;

typedef struct GamePlay{
    GameScreen screen;
    GameScreen pending_screen;
    bool transition_pending;
    float transition_counter;
    ScreenTransition transition;

    Dungeon dungeon[DUNGEON_COUNT];
    int current_dungeon_depth;

    Entity player;
    bool dead;
    bool dirty;
    Camera2D camera;
    ScreenShake screen_shake;
    RangedMode ranged_mode;

    bool inventory;
    bool dialog;

    char message_info[STR_LENGHT_MESSAGE];

    RenderUI render_ui;
}GamePlay;

void gameplay_restart_playing(GamePlay *self);
void gameplay_init(GamePlay *self);
void gameplay_update(GamePlay *self, float delta_time);
void gameplay_render(GamePlay *self, float delta_time);

Dungeon *gameplay_current_dungeon(GamePlay *self);
void gameplay_on_enter_stairs(GamePlay *self);
void gameplay_on_exit_stairs_up(GamePlay *self);


#endif // GAMEPLAY_H
