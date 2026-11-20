#ifndef RENDER_GAME_UI_H
#define RENDER_GAME_UI_H

typedef struct GamePlay GamePlay;

typedef struct RenderUI{
    float npc_status_animation;
    float npc_status_animation_counter;

    int page_start;
    int page_end;
    bool dialog_open;
    int next_line;
}RenderUI;

void render_game_ui_gameplay_system(GamePlay *gameplay, float delta_time);
void render_game_ui_system_screen(GamePlay *gameplay);

#endif // RENDER_GAME_UI_H
