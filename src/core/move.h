#ifndef MOVER_H
#define MOVER_H

typedef struct {
    Vector2 pos, prev;  /* float world pixels */
    Vector2 vel;        /* pixels per tick */
    float speed;
    float accel;
    float friction;
} Mover;

static inline void mover_move(Mover *m)
{
    m->prev = m->pos;
    m->pos.x += m->vel.x;
    m->pos.y += m->vel.y;
}

/* position to draw this frame */
static inline Vector2 mover_lerp(const Mover *m, float alpha)
{
    return (Vector2){
        m->prev.x + (m->pos.x - m->prev.x) * alpha,
        m->prev.y + (m->pos.y - m->prev.y) * alpha,
    };
}

static inline float approach(float v, float target, float amount)
{
    if (v < target) return fminf(v + amount, target);
    return fmaxf(v - amount, target);
}

#endif