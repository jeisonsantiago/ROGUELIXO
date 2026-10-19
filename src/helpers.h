#ifndef HELPERS_H
#define HELPERS_H


typedef struct Vector2i{
    int x;
    int y;
}Vector2i;


bool coin_flip();
Vector2i transform_octant(int x,  int y, int octant);

static Vector2i neighbours_8[8] = {
    // x y
    {-1,-1},
    {0,-1},
    {1,-1},
    {1,0},
    {1,1},
    {0,1},
    {-1,1},
    {-1,0}
};

static Vector2i neighbours_4[4] = {
    // x y
    {0,-1},
    {1,0},
    {0,1},
    {-1,0}
};

bool ray_hits_rectangle(Vector2 origin, Vector2 dir, Rectangle rect, Vector2 *hit);

Vector2 point_at_distance(Vector2 a, Vector2 b, float distance);

bool sgn(int x);

bool has_flag(uint16_t flags, uint16_t flag);

int flag_index(unsigned int flag);

bool chance(int percentage);

#endif // HELPERS_H
