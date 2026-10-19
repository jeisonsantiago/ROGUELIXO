#include "monster.h"


#include "entity.h"
#include "map.h"
#include "gameplay.h"
#include "helpers.h"

void monster_wander(Entity *self, Map *map)
{
    static const int dx[4] = {  0,  0, -1, +1 };
    static const int dy[4] = { -1, +1,  0,  0 };

    int choice = GetRandomValue(0,4);
    if(choice < 4){
        entity_try_move(self,dx[choice],dy[choice],map);
    }
}

void monster_entity_setup(Entity *entity){
    strcpy(entity->name,"monster");
    entity->max_hp = 2;
    entity->hp = entity->max_hp;
    entity->damage = 1;
    entity->alive = true;
}

Entity *monster_at(GamePlay *gameplay, int x, int y){

    for (int i = 0; i < gameplay->dungeon.monster_count; ++i) {
        Entity *monster = &gameplay->dungeon.monster[i];

        if(monster->alive && monster->x == x && monster->y == y) return monster;
    }

    return NULL;
}

void monster_update(GamePlay *gameplay){

    for (int i = 0; i < gameplay->dungeon.monster_count; ++i) {
        Entity *monster = &gameplay->dungeon.monster[i];

        if(!gameplay->dungeon.monster->alive) return;

        // TraceLog(LOG_INFO, "monster at: %i %i", gameplay->monster.x, gameplay->monster.y);


        // warder if not visible
        if((int)entity_distance(&gameplay->player, monster) > gameplay->player.data.player.fov_distance){

        // if(!gameplay->dungeon.map.tiles[monster->y][monster->x].visible){
            // TraceLog(LOG_INFO, "monster at: %i %i WANDERING", gameplay->monster->x, gameplay->monster->y);

            monster_wander(monster,&gameplay->dungeon.map);
            // return; // continue in entity loop
        }else{
            // TraceLog(LOG_INFO,"distance: %f, fov: %i", entity_distance(&gameplay->player, monster), gameplay->player.data.player.fov_distance);

            // TraceLog(LOG_INFO, "monster at: %i %i HUNTING", gameplay->monster->x, gameplay->monster->y);
            // check distance from player
            int ddx = gameplay->player.x - monster->x;
            int ddy = gameplay->player.y - monster->y;

            // adjacent to the player?
            if(abs(ddx) + abs(ddy) == 1){
                entity_attack(monster, &gameplay->player);
                return;
            }

            int best_x = monster->x;
            int best_y = monster->y;
            int best_value = gameplay->dungeon.map.dist[monster->y][monster->x];

            // static int directions[4][2] = {
            //     { 1,  0 },
            //     {-1,  0 },
            //     { 0,  1 },
            //     { 0, -1 }
            // };

            // get best value
            for (int d = 0; d < 4; ++d) {
                int nx = monster->x + directions[d][0];
                int ny = monster->y + directions[d][1];

                if(!map_is_inside(&gameplay->dungeon.map,nx, ny)) continue;
                if(gameplay->dungeon.map.tiles[ny][nx].type != TILE_TYPE_FLOOR) continue;
                if(nx == gameplay->player.x && ny == gameplay->player.y) continue; // never stack them
                if(monster_at(gameplay, nx, ny)) continue; // cant stack monster as well

                int v = gameplay->dungeon.map.dist[ny][nx];
                if (v == UNREACHABLE) continue;

                if(v < best_value){
                    best_value = v;
                    best_x = nx;
                    best_y = ny;
                }
            }

            monster->x = best_x;
            monster->y = best_y;

        }
        // TODO: update pathfinding!!!
        // Otherwise close the distance: step along the longer axis first,
        // fall back to the other axis if that way is blocked.
        // int sx = (abs(ddx) >= abs(ddy)) ? sgn(ddx) : 0;
        // int sy = (sx == 0) ? sgn(ddy) : 0;

        // if(!entity_step_if_free(monster, gameplay, sx, sy)){
        //     entity_step_if_free(monster, gameplay, sgn(ddx)-sx, sgn(ddy)-sy); // try other axis
        // }
    }


}