// #include "shared_context.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

#include "core/input.h"
#include "core/time.h"

#include "game.h"

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1280, 720, "Gauntlet");
    SetWindowMinSize(GAME_WIDTH, GAME_HEIGHT);
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    game_init();

    while (!WindowShouldClose()) {
        float sw = (float)GetScreenWidth(), sh = (float)GetScreenHeight();
        float scale = MIN(sw / GAME_WIDTH, sh / GAME_HEIGHT);
        float ox = (sw - GAME_WIDTH  * scale) * 0.5f;
        float oy = (sh - GAME_HEIGHT * scale) * 0.5f;

        Vector2 m  = GetMousePosition();
        Vector2 vm = { (m.x - ox) / scale, (m.y - oy) / scale };
        vm = Vector2Clamp(vm, (Vector2){ 0, 0 },
                          (Vector2){ GAME_WIDTH, GAME_HEIGHT });

        input_update(vm);
        time_begin_frame();

        while (time_step()) {
            game_update(FIXED_DT);
            input_end_step();
        }

        BeginTextureMode(target);
            ClearBackground(BLACK);
            game_draw();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(target.texture,
                (Rectangle){ 0, 0, (float)target.texture.width,
                                   (float)-target.texture.height },
                (Rectangle){ ox, oy, GAME_WIDTH * scale, GAME_HEIGHT * scale },
                (Vector2){ 0, 0 }, 0.0f, WHITE);
            game_draw_hud(scale, ox, oy);
            DrawFPS(10, 10);
        EndDrawing();
    }

    game_shutdown();
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}