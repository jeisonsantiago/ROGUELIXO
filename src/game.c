#include "game.h"
#include "game/entity.h"
#include "core/input.h"

#define WORLD_W 2000
#define WORLD_H 2000
#define DEAD_W  24
#define DEAD_H  16

typedef struct {
    int   x, y;        /* whole pixels */
    int   radius;
    float step;        /* px per tick, can be 0.5, 1, 1.5, 2... */
    float step_rem;
    float flash;
} Player;

typedef struct {
    EntityPool entities;
    int        level;
    int        score;
    Player     player;
    Camera2D   camera;
    int        cam_x, cam_y;
} GameState;

static GameState g;

void game_init(void) {
    Player *p = &g.player;
    p->x = GAME_WIDTH / 2;
    p->y = GAME_HEIGHT / 2;
    p->radius   = 6;
    p->step     = 1.f;
    p->step_rem = 0.0f;
    p->flash    = 0.0f;

    g.cam_x = p->x;
    g.cam_y = p->y;
    g.camera.offset   = (Vector2){ GAME_WIDTH / 2.0f, GAME_HEIGHT / 2.0f };
    g.camera.rotation = 0.0f;
    g.camera.zoom     = 1.0f;
}

void game_update(float dt) {
    Player *p = &g.player;
    Vector2 dir = input_move_dir_raw();   /* -1, 0, 1 per axis */

    p->step_rem += p->step;
    int s = (int)p->step_rem;
    p->step_rem -= s;

    p->x += (int)dir.x * s;
    p->y += (int)dir.y * s;

    if (p->x < p->radius)           p->x = p->radius;
    if (p->x > WORLD_W - p->radius) p->x = WORLD_W - p->radius;
    if (p->y < p->radius)           p->y = p->radius;
    if (p->y > WORLD_H - p->radius) p->y = WORLD_H - p->radius;

    /* deadzone camera, whole pixels */
    if (p->x > g.cam_x + DEAD_W) g.cam_x = p->x - DEAD_W;
    if (p->x < g.cam_x - DEAD_W) g.cam_x = p->x + DEAD_W;
    if (p->y > g.cam_y + DEAD_H) g.cam_y = p->y - DEAD_H;
    if (p->y < g.cam_y - DEAD_H) g.cam_y = p->y + DEAD_H;

    if (input_pressed(ACTION_FIRE)) p->flash = 0.15f;
    if (p->flash > 0) p->flash -= dt;
}

void game_draw(void) {
    Player *p = &g.player;
    g.camera.target = (Vector2){ (float)g.cam_x, (float)g.cam_y };

    BeginMode2D(g.camera);
        for (int y = 0; y < WORLD_H; y += 16)
            for (int x = 0; x < WORLD_W; x += 16)
                if (((x / 16) + (y / 16)) % 2 == 0)
                    DrawRectangle(x, y, 16, 16, (Color){ 30, 30, 40, 255 });
        DrawRectangleLines(0, 0, WORLD_W, WORLD_H, RED);

        int r = p->radius;
        DrawRectangle(p->x - r, p->y - r, r * 2, r * 2,
                      p->flash > 0 ? WHITE : YELLOW);
    EndMode2D();
}

void game_draw_hud(float scale, float ox, float oy) {
    DrawText("pixel step", (int)(ox + 4 * scale), (int)(oy + 4 * scale),
             (int)(10 * scale), GRAY);
}

void game_shutdown(void) { }