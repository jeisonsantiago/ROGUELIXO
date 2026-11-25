#include "helpers.h"

bool coin_flip(){
    return GetRandomValue(0,1);
}



Vector2i transform_octant(int x,  int y, int octant){
    switch (octant) {
    case 0:return (Vector2i){x,y};
    case 1: return (Vector2i){y, x};
    case 2: return (Vector2i){y, -x};
    case 3: return (Vector2i){x, -y};
    case 4: return (Vector2i){-x,  -y};
    case 5: return (Vector2i){-y,  -x};
    case 6: return (Vector2i){-y, x};
    case 7: return (Vector2i){-x, y};
    }

    return (Vector2i){x,y};
}

bool ray_hits_rectangle(Vector2 origin, Vector2 dir, Rectangle rect, Vector2 *hit)
{
    float tMin = 0.0f;
    float tMax = FLT_MAX;

    float rectMin[2] = { rect.x, rect.y };
    float rectMax[2] = { rect.x + rect.width, rect.y + rect.height };
    float o[2] = { origin.x, origin.y };
    float d[2] = { dir.x, dir.y };

    for (int axis = 0; axis < 2; axis++) {
        if (d[axis] == 0.0f) {
            if (o[axis] < rectMin[axis] || o[axis] > rectMax[axis]) return false;
        } else {
            float t1 = (rectMin[axis] - o[axis]) / d[axis];
            float t2 = (rectMax[axis] - o[axis]) / d[axis];

            if (t1 > t2) {
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }

            if (t1 > tMin) tMin = t1;
            if (t2 < tMax) tMax = t2;
            if (tMin > tMax) return false;
        }
    }

    if (hit) {
        hit->x = origin.x + tMin * dir.x;
        hit->y = origin.y + tMin * dir.y;
    }
    return true;
}
Vector2 point_at_distance(Vector2 a, Vector2 b, float distance)
{
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float length = sqrtf(dx * dx + dy * dy);

    if (length == 0.0f) {
        return a; // The points are identical
    }

    float t = distance / length;

    return (Vector2) {
        a.x + dx * t,
                a.y + dy * t
    };
}

bool sgn(int x)
{
    return (x > 0) - (x < 0); // 1, 0, or -1
}

bool has_flag(uint16_t flags, uint16_t flag)
{
    return (flags & flag) != 0;
}

int flag_index(unsigned int flag) {
    int index = 0;

    while (flag > 1u) {
        flag >>= 1;
        index++;
    }

    return index;
}

bool chance(int percentage)
{
    if (percentage <= 0) return false;
    if (percentage >= 100) return true;

    return GetRandomValue(1, 100) <= percentage;
}
