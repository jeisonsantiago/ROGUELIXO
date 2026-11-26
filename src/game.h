#ifndef GAME_H
#define GAME_H


// #define GAME_WIDTH  320
// #define GAME_HEIGHT 180


#define GAME_WIDTH  640
#define GAME_HEIGHT 360

// #define GAME_WIDTH  320
// #define GAME_HEIGHT 180

void game_init(void);
void game_update(float dt);
void game_draw(void);
void game_draw_hud(float scale, float ox, float oy);
void game_shutdown(void);

#endif // GAME_H
