#ifndef PCH_H
#define PCH_H

#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define STR_LENGHT 30


static int directions[4][2] = {
    { 1,  0 },
    {-1,  0 },
    { 0,  1 },
    { 0, -1 }
};

static const int UNREACHABLE = 1 << 30;

#endif // PCH_H
