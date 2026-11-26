#include "entity.h"

void entity_pool_clear(EntityPool *p)
{
    memset(p, 0, sizeof(*p));
}
int entity_spawn(EntityPool *p, EntityType type, Vector2 pos)
{
    for (int i = 0; i < MAX_ENTITIES; i++) {
        Entity *e = &p->items[i];
        if (!e->active) {
            memset(e, 0, sizeof(*e));
            e->active = true;
            e->type   = type;
            e->pos    = pos;
            e->owner  = -1;
            if (i + 1 > p->count) p->count = i + 1;
            return i;
        }
    }
    return -1; /* pool full */
}
void entity_kill(EntityPool *p, int id)
{
    if (id >= 0 && id < MAX_ENTITIES) p->items[id].active = false;
}
Entity *entity_get(EntityPool *p, int id)
{
    if (id < 0 || id >= MAX_ENTITIES || !p->items[id].active) return NULL;
    return &p->items[id];
}
void entity_update_all(EntityPool *p, float dt)
{
    for (int i = 0; i < p->count; i++) {
        Entity *e = &p->items[i];
        if (!e->active) continue;
        switch (e->type) {
        case ENTITY_PLAYER:     /* player_update(e, dt); */ break;
        case ENTITY_GRUNT:      /* grunt_update(e, dt);  */ break;
        case ENTITY_PROJECTILE:
            e->pos.x += e->vel.x * dt;
            e->pos.y += e->vel.y * dt;
            e->timer -= dt;
            if (e->timer <= 0) e->active = false;
            break;
        default: break;
        }
    }
}