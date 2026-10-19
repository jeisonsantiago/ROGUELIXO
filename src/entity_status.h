#ifndef ENTITY_STATUS_H
#define ENTITY_STATUS_H

typedef struct Entity Entity;

typedef enum Status{
    STATUS_NONE,
    STATUS_POISONED,
    STATUS_REGEN,
    STATUS_COUNT
}Status;


void status_add(Entity *entity, Status status, int turns);
bool status_has(Entity *entity, Status status);
void status_update(Entity *entity);
void status_apply(Entity *attack, Entity *defence); // by behaivior
Color status_color(Entity *entity);

#endif // ENTITY_STATUS_H
