#include "input.h"

// key bindins
// shared_context()->input_mapper.key_bindins[ACTION_MOVE_UP]          = KEY_W;
// shared_context()->input_mapper.key_bindins[ACTION_MOVE_DOWN]        = KEY_S;
// shared_context()->input_mapper.key_bindins[ACTION_MOVE_LEFT]        = KEY_A;
// shared_context()->input_mapper.key_bindins[ACTION_MOVE_RIGHT]       = KEY_D;
// shared_context()->input_mapper.key_bindins[ACTION_ATTACK]           = KEY_SPACE;
// shared_context()->input_mapper.key_bindins[ACTION_USE]              = KEY_E;

static const int bindings[ACTION_COUNT][2] = {
    [ACTION_UP]    = { KEY_W, KEY_UP },
    [ACTION_DOWN]  = { KEY_S, KEY_DOWN },
    [ACTION_LEFT]  = { KEY_A, KEY_LEFT },
    [ACTION_RIGHT] = { KEY_D, KEY_RIGHT },
    [ACTION_FIRE]  = { KEY_SPACE, KEY_J },
};

static bool down[ACTION_COUNT];
static bool pressed[ACTION_COUNT];
static Vector2 mouse;

void input_update(Vector2 virtual_mouse) {
    mouse = virtual_mouse;
    for (int a = 0; a < ACTION_COUNT; a++) {
        int k0 = bindings[a][0], k1 = bindings[a][1];
        down[a] = IsKeyDown(k0) || IsKeyDown(k1);
        if (IsKeyPressed(k0) || IsKeyPressed(k1))
            pressed[a] = true;   /* stays set until a fixed step uses it */
    }
}
void input_end_step(void) {
    for (int a = 0; a < ACTION_COUNT; a++) pressed[a] = false;
}

bool input_down(Action a)    { return down[a]; }
bool input_pressed(Action a) { return pressed[a]; }
Vector2 input_mouse(void)    { return mouse; }

Vector2 input_move_dir(void) {
    Vector2 d = { 0, 0 };
    if (down[ACTION_UP])    d.y -= 1;
    if (down[ACTION_DOWN])  d.y += 1;
    if (down[ACTION_LEFT])  d.x -= 1;
    if (down[ACTION_RIGHT]) d.x += 1;
    return Vector2Normalize(d);  /* diagonals aren't faster; zero stays zero */
}

Vector2 input_move_dir_raw(void) {
    Vector2 d = { 0, 0 };
    if (down[ACTION_UP])    d.y -= 1;
    if (down[ACTION_DOWN])  d.y += 1;
    if (down[ACTION_LEFT])  d.x -= 1;
    if (down[ACTION_RIGHT]) d.x += 1;
    return d;
}