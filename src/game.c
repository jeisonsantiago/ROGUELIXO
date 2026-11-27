#include "game.h"
#include "game/entity.h"
#include "core/move.h"
#include "core/input.h"

#define WORLD_W 2000
#define WORLD_H 2000

// #define PLAYER_MAX   0.5f   /* px per tick */
// #define PLAYER_ACCEL 0.25f
// #define PLAYER_FRIC  0.20f
#define CAM_FOLLOW   0.12f  /* 0..1 per tick */

#define MAX_ENEMIES 8

typedef struct { Mover m; float size; float flash; } Player;
typedef struct { Mover m; float size; float speed; bool active; } Enemy;

typedef struct {
    EntityPool entities;
    int level, score;
    Player  player;
    Enemy   enemies[MAX_ENEMIES];
    Vector2 cam, cam_prev;
} GameState;

static GameState g;

static void clamp_mover(Mover *m, float half)
{
    if (m->pos.x < half)           { m->pos.x = half;           m->vel.x = 0; }
    if (m->pos.x > WORLD_W - half) { m->pos.x = WORLD_W - half; m->vel.x = 0; }
    if (m->pos.y < half)           { m->pos.y = half;           m->vel.y = 0; }
    if (m->pos.y > WORLD_H - half) { m->pos.y = WORLD_H - half; m->vel.y = 0; }
}

void game_init(void)
{
    g = (GameState){0};

    g.player.m.pos = g.player.m.prev = (Vector2){ 200, 200 };
    g.player.size  = 12;

    g.player.m.speed = 0.5f;   /* px per tick */
    g.player.m.accel = 0.25f;
    g.player.m.friction = 0.20f;

    g.enemies[0] = (Enemy){ .m = { .pos = {120,120}, .prev = {120,120} },
                            .size = 12, .speed = 0.3f, .active = true };
    g.enemies[1] = (Enemy){ .m = { .pos = {300,120}, .prev = {300,120} },
                            .size = 12, .speed = 0.1f, .active = true };

    g.cam = g.cam_prev = g.player.m.pos;
}

static void update_player(Player *p, float dt)
{
    Vector2 dir = input_move_dir();
    p->m.vel.x = approach(p->m.vel.x, dir.x * p->m.accel,
                          dir.x != 0 ? p->m.accel : p->m.friction);
    p->m.vel.y = approach(p->m.vel.y, dir.y * p->m.speed,
                          dir.y != 0 ? p->m.accel : p->m.friction);
    mover_move(&p->m);
    clamp_mover(&p->m, p->size / 2);

    if (p->flash > 0) p->flash -= dt;
    if (input_pressed(ACTION_FIRE)) p->flash = 0.1f;
}

static void update_enemy(Enemy *e, const Player *p)
{
    Vector2 d = Vector2Subtract(p->m.pos, e->m.pos);
    float len = Vector2Length(d);
    e->m.vel = len > 1.0f ? Vector2Scale(d, e->speed / len) : (Vector2){0, 0};
    mover_move(&e->m);
    clamp_mover(&e->m, e->size / 2);
}

void game_update(float dt)
{
    update_player(&g.player, dt);
    for (int i = 0; i < MAX_ENEMIES; i++)
        if (g.enemies[i].active) update_enemy(&g.enemies[i], &g.player);

    g.cam_prev = g.cam;
    g.cam = Vector2Lerp(g.cam, g.player.m.pos, CAM_FOLLOW);
}

static float view_scale(void)
{
    return fminf(GetScreenWidth()  / (float)VIEW_W,
                 GetScreenHeight() / (float)VIEW_H);
}

void game_draw(float alpha)
{
    float scale = view_scale();

    Vector2 c = Vector2Lerp(g.cam_prev, g.cam, alpha);
    /* snap camera to whole SCREEN pixels: avoids tile seams, still smooth */
    c.x = floorf(c.x * scale) / scale;
    c.y = floorf(c.y * scale) / scale;

    Camera2D cam = {0};
    cam.target = c;
    cam.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    cam.zoom   = scale;

    BeginMode2D(cam);

        /* world */
        int grid_size = 16;
        for (int y = 0; y < WORLD_H; y += grid_size)
            for (int x = 0; x < WORLD_W; x += grid_size)
                if (((x / grid_size) + (y / grid_size)) % 2 == 0)
                    DrawRectangle(x, y, grid_size, grid_size, (Color){ 40, 40, 60, 255 });
        DrawRectangleLines(0, 0, WORLD_W, WORLD_H, RED);

        /* actors, float positions */
        for (int i = 0; i < MAX_ENEMIES; i++) {
            Enemy *e = &g.enemies[i];
            if (!e->active) continue;
            Vector2 p = mover_lerp(&e->m, alpha);
            DrawRectangleV((Vector2){ p.x - e->size/2, p.y - e->size/2 },
                           (Vector2){ e->size, e->size }, MAROON);
        }

        Player *pl = &g.player;
        Vector2 p = mover_lerp(&pl->m, alpha);
        DrawRectangleV((Vector2){ p.x - pl->size/2, p.y - pl->size/2 },
                       (Vector2){ pl->size, pl->size },
                       pl->flash > 0 ? YELLOW : SKYBLUE);
    EndMode2D();

    /* letterbox: black bars outside the 320x180 view */
    float sw = (float)GetScreenWidth(), sh = (float)GetScreenHeight();
    float vw = VIEW_W * scale, vh = VIEW_H * scale;
    float ox = (sw - vw) / 2, oy = (sh - vh) / 2;
    DrawRectangle(0, 0, (int)ox + 1, (int)sh, BLACK);
    DrawRectangle((int)(ox + vw), 0, (int)ox + 1, (int)sh, BLACK);
    DrawRectangle(0, 0, (int)sw, (int)oy + 1, BLACK);
    DrawRectangle(0, (int)(oy + vh), (int)sw, (int)oy + 1, BLACK);
}

void game_draw_hud(void)
{
    DrawText(TextFormat("one render  vel %.2f %.2f",
                        g.player.m.vel.x, g.player.m.vel.y),
             20, 40, 20, RAYWHITE);
}

void game_shutdown(void) {}