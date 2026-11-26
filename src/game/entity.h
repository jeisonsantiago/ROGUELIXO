#ifndef ENTITY_H
#define ENTITY_H

#define MAX_ENTITIES 1024

typedef enum {
    ENTITY_NONE = 0,
    ENTITY_PLAYER,
    ENTITY_GRUNT,
    ENTITY_GHOST,
    ENTITY_SPAWNER,
    ENTITY_PROJECTILE,
    ENTITY_PICKUP,
} EntityType;

typedef struct {
    EntityType type;
    bool    active;
    Vector2 pos;
    Vector2 vel;
    float   radius;
    float   speed;
    int     hp;
    int     owner;   /* index of who fired it, -1 if none */
    float   timer;   /* cooldown, lifetime, spawn timer... */
} Entity;

typedef struct {
    Entity items[MAX_ENTITIES];
    int    count;    /* highest used slot + 1 */
} EntityPool;

void    entity_pool_clear(EntityPool *p);
int     entity_spawn(EntityPool *p, EntityType type, Vector2 pos);
void    entity_kill(EntityPool *p, int id);
Entity *entity_get(EntityPool *p, int id);
void    entity_update_all(EntityPool *p, float dt);

#endif // ENTITY_H
