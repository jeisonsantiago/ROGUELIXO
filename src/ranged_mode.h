#ifndef RANGED_MODE_H
#define RANGED_MODE_H

#include "helpers.h"

typedef struct GamePlay GamePlay;

typedef struct RangedMode{
    bool enabled;
    Vector2i position;
    bool can_shoot;
}RangedMode;

bool ranged_mode_update(GamePlay *gameplay);
void ranged_mode_render(GamePlay *gameplay);

#endif // RANGED_MODE_H
