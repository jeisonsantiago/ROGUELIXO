#include "raylib.h"
#include "game.h"
#include "core/time.h"
#include "core/input.h"

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "gauntlet");
    SetTargetFPS(60);

    game_init();

    while (!WindowShouldClose()) {
        float sw = (float)GetScreenWidth(), sh = (float)GetScreenHeight();
        float scale = sw / VIEW_W < sh / VIEW_H ? sw / VIEW_W : sh / VIEW_H;
        float ox = (sw - VIEW_W * scale) / 2, oy = (sh - VIEW_H * scale) / 2;

        Vector2 m  = GetMousePosition();
        Vector2 vm = { (m.x - ox) / scale, (m.y - oy) / scale };

        input_update(vm);
        time_begin_frame();
        while (time_step()) {
            game_update(FIXED_DT);
            input_end_step();
        }
        float alpha = time_alpha();

        BeginDrawing();
            ClearBackground(BLACK);
            game_draw(alpha);
            game_draw_hud();
            DrawFPS(10, 10);
        EndDrawing();
    }

    game_shutdown();
    CloseWindow();
    return 0;
}