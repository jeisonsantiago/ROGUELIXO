#ifndef GAME_H
#define GAME_H


// #define GAME_WIDTH  1280
// #define GAME_HEIGHT 720


// /*1280*/, 720
// #define GAME_WIDTH  640
// #define GAME_HEIGHT 360

// #define GAME_WIDTH  320
// #define GAME_HEIGHT 180


#define VIEW_W 320 /* world pixels visible, same on every monitor */
#define VIEW_H 180

void game_init(void);
void game_update(float dt);
void game_draw(float alpha);
void game_draw_hud(void);
void game_shutdown(void);

#endif // GAME_H
