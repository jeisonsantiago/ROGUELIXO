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

    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    for (int i = 0; i < dungeon->monster_count; ++i) {
        Entity *monster = &dungeon->monster[i];

        if(entity_distance(&gameplay->player, monster) < gameplay->player.data.player.fov_distance){
            if(monster->alive && monster->x == x && monster->y == y) return monster;
        }
    }

    return NULL;
}

void monster_update(GamePlay *gameplay){

    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    for (int i = 0; i < dungeon->monster_count; ++i) {
        Entity *monster = &dungeon->monster[i];

        if(!monster->alive) continue;

        if(monster->entity_type == ENTITY_TYPE_NPC) continue;

        TraceLog(LOG_INFO,"monster update:%i %i",monster->x, monster->y);

        // warder if not visible
        if((int)entity_distance(&gameplay->player, monster) > gameplay->player.data.player.fov_distance){

        // if(!gameplay->dungeon.map.tiles[monster->y][monster->x].visible){
            // TraceLog(LOG_INFO, "monster at: %i %i WANDERING", gameplay->monster->x, gameplay->monster->y);

            monster_wander(monster,&dungeon->map);
            // return; // continue in entity loop
        }else{
            // TraceLog(LOG_INFO,"distance: %f, fov: %i", entity_distance(&gameplay->player, monster), gameplay->player.data.player.fov_distance);

            // TraceLog(LOG_INFO, "monster at: %i %i HUNTING", gameplay->monster->x, gameplay->monster->y);
            // check distance from player
            int ddx = gameplay->player.x - monster->x;
            int ddy = gameplay->player.y - monster->y;

            // adjacent to the player?
            if(abs(ddx) + abs(ddy) == 1){
                //entity_attack(monster, &gameplay->player);
                entity_start_attacking(monster,&gameplay->player);
                return;
            }

            int best_x = monster->x;
            int best_y = monster->y;
            int best_value = dungeon->map.dist[monster->y][monster->x];

            // static int directions[4][2] = {
            //     { 1,  0 },
            //     {-1,  0 },
            //     { 0,  1 },
            //     { 0, -1 }
            // };

            int dx = 0;
            int dy = 0;

            // get best value
            for (int d = 0; d < 4; ++d) {
                int nx = monster->x + directions[d][0];
                int ny = monster->y + directions[d][1];

                if(!map_is_inside(&dungeon->map,nx, ny)) continue;
                if(dungeon->map.tiles[ny][nx].type != TILE_TYPE_FLOOR) continue;
                if(nx == gameplay->player.x && ny == gameplay->player.y) continue; // never stack them
                if(monster_at(gameplay, nx, ny)) continue; // cant stack monster as well

                int v = dungeon->map.dist[ny][nx];
                if (v == UNREACHABLE) continue;

                if(v < best_value){
                    best_value = v;
                    best_x = nx;
                    best_y = ny;

                    dx = directions[d][0];
                    dy = directions[d][1];
                }
            }

            bool move = entity_try_move(monster,dx,dy,&dungeon->map);

            // monster->x = dx;
            // monster->y = dy;

        }        
    }


}