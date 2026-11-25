#include "gameplay.h"
#include "shared_context.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    // Enable config flags for resizable window and vertical synchro
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - window letterbox");
    SetWindowMinSize(1280, 720);
    SetExitKey(0);

    // set random seed
    SetRandomSeed((unsigned int)time(NULL));

    int gameScreenWidth = 1280;
    int gameScreenHeight = 720;

    // Render texture initialization, used to hold the rendering result so we can easily resize it
    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);  // Texture scale filter to use

    Color colors[10] = { 0 };
    for (int i = 0; i < 10; i++) colors[i] = (Color){ GetRandomValue(100, 250), GetRandomValue(50, 150), GetRandomValue(10, 100), 255 };

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Map map;

    asset_manager_init(&shared_context()->asset_manager);
    asset_manager_load_all(&shared_context()->asset_manager);

    // game logic
    // map_init(&map);

    GamePlay gameplay;
    gameplay_init(&gameplay);
    gameplay.screen = SCREEN_MENU;

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Compute required framebuffer scaling
        float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });

        float delta_time = GetFrameTime();

        // loop over actions
        for (int i = 0; i < ACTION_COUNT; ++i) {
            // look over the key related to this index
            KeyboardKey bound_key = shared_context()->input_mapper.key_bindins[i];

            // SDL_Log("raw: %i",raw_keyboard[i]);
            // if key is held down set actopm
            shared_context()->input_state.time_out_actions_down[i] = IsKeyDown(bound_key);
            shared_context()->input_state.time_out_actions_pressed[i] = IsKeyPressed(bound_key);
        }

        if(IsKeyPressed(KEY_SPACE)){
            gameplay_init(&gameplay);
        }

        // update
        gameplay_update(&gameplay,delta_time);


        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
            ClearBackground(BLACK);  // Clear render texture background color


            // DrawText("If executed inside a window,\nyou can resize the window,\nand see the screen scaling!", 10, 25, 20, RED);
            // DrawText(TextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, GREEN);
            // DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)virtualMouse.x, (int)virtualMouse.y), 350, 55, 20, BLUE);

            // for (int i = 0; i < map.rooms_count; ++i) {
            //     Room *room = &map.rooms[i];

                // DrawRectangleLines(
                //             room->x * TILE_SIZE,
                //             room->y * TILE_SIZE,
                //             room->w * TILE_SIZE,
                //             room->h * TILE_SIZE,
                //             BLUE);
                // TraceLog(LOG_INFO,"%i %i %i %i",room->x,room->y,room->w,room->h);
            // }
        gameplay_render(&gameplay,delta_time);

        // for (int x = 0; x < MAP_WIDTH; ++x) {
        //     for (int y = 0; y < MAP_HEIGHT; ++y) {
        //         if(dungeon.map.tiles[y][x].type == TILE_TYPE_FLOOR){
        //             DrawRectangleLines(
        //                         x * TILE_SIZE,
        //                         y * TILE_SIZE,
        //                         TILE_SIZE,
        //                         TILE_SIZE,
        //                         RED);
        //         }
        //     }
        // }



        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);     // Clear screen background

            // Draw render texture to screen, properly scaled
            DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
                           (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);        // Unload render texture

    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}