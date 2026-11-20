#ifndef MONSTER_H
#define MONSTER_H

typedef struct Entity Entity;
typedef struct Map Map;
typedef struct GamePlay GamePlay;



typedef struct Monster{
    // Entity entity;

    // Vector2 edges[360];
    // int edges_count;

}Monster;

void monster_entity_setup(Entity *entity);
void monster_wander(Entity *self, Map *map);
Entity *monster_at(GamePlay *gameplay, int x, int y);
void monster_update(GamePlay *gameplay);
// void monster_create(GamePlay *gameplay);



#endif // MONSTER_H
