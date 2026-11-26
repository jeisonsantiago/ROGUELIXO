#include "time.h"

static float accumulator = 0.0f;
void time_begin_frame(void) {
    float dt = GetFrameTime();
    if (dt > 0.25f) dt = 0.25f;   /* avoid a spiral after a hitch */
    accumulator += dt;
}
int time_step(void) {
    if (accumulator >= FIXED_DT) {
        accumulator -= FIXED_DT;
        return 1;
    }
    return 0;
}

float time_alpha(void) { return accumulator / FIXED_DT; }