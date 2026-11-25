#include "player.h"

#include "map.h"
// #include "shared_context.h"
#include "entity.h"
#include "gameplay.h"
#include "helpers.h"


static void equip_ranged(Entity *entity){

    Item it ={
        .identified = true,
        .type = ITEM_TYPE_WEAPON,
        .x = 0,
        .y = 0,
        .data.weapon = (Weapon){.amount = 2, .type = WEAPON_RANGED},
    };

    it.sprite_index = (16*4+9);
    // it.sprite_index = ();
    strcpy(it.description,TextFormat("Bow Fuckery: %i STR",it.data.weapon.amount));

    inventory_add_item(&entity->inventory, it);

    entity_equip(entity,0);

    entity->state = STATE_IDLE;
}

void player_entity_setup(Entity *entity){
    strcpy(entity->name,"player");
    entity->damage = 5;
    entity->data.player.fov_distance = 10;
    entity->timer_attacking = 0.3f;

    // set ranged equiped
    equip_ranged(entity);
}

void player_calculate_fov(GamePlay *gameplay)
{
    Entity *player = &gameplay->player;
    // calculate FOV // first octant
    int max_distance = gameplay->player.data.player.fov_distance;

    // get current dungeon;
    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    dungeon->map.tiles[gameplay->player.y][gameplay->player.x].visible = true;
    int x = 0;
    int y = 0;

    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            dungeon->map.tiles[y][x].visible = false;
        }
    }

    Vector2 visibles[(int)powf(max_distance,4)];
    int visibles_count = 0;

    for (int oct = 0; oct < 8; ++oct) {
        for (int y = 0; y < max_distance; ++y) {
            for (int x = 0; x <= y; ++x) {

                // transform_octant(&xx,&yy,i);

                Vector2i pos = transform_octant(x,y,oct);

                int vx = gameplay->player.x + pos.x;
                int vy = gameplay->player.y - pos.y;

                if(!map_is_inside(&dungeon->map,vx,vy)) continue;

                int dx = vx - gameplay->player.x;
                int dy = vy - gameplay->player.y;

                if(dx * dx + dy * dy < (max_distance * max_distance)){
                    dungeon->map.tiles[vy][vx].visible = true;
                    visibles[visibles_count++] = (Vector2){vx,vy};
                }
            }
        }
    }

    for (int i = 0; i < visibles_count; ++i) {
        dungeon->map.tiles[(int)visibles[i].y][(int)visibles[i].x].visible = false;
    }

    // self->player.data.player.edges_count = 0;

    // float sx = self->player.x + 0.5f;
    // float sy = self->player.y + 0.5f;
    float sx = gameplay->player.x + 0.5f;
    float sy = gameplay->player.y + 0.5f;

    Vector2 player_center = {sx,sy};

    for (int degrees = 0; degrees < 360; degrees += 5) {
        float radians = degrees * PI / 180.0f;
        Vector2 direction = { cosf(radians), sinf(radians) };

        // Example endpoint 0.5 units from the player
        Vector2 endpoint = {
            sx + max_distance * direction.x,
            sy + max_distance * direction.y
        };

        for (int p = 0; p < 6; ++p) {
            Vector2 check_point = point_at_distance(player_center,endpoint,p);

            bool wall_reached =false;

            for (int r = 0; r < visibles_count; ++r) {

                Rectangle rect = { visibles[r].x, visibles[r].y,1,1};

                if (CheckCollisionPointRec(check_point,rect)) {

                    int tile_x = (int)floorf(visibles[r].x);
                    int tile_y = (int)floorf(visibles[r].y);

                    // Check these indices are within the map bounds before indexing.
                    dungeon->map.tiles[tile_y][tile_x].visible = true;
                    dungeon->map.tiles[tile_y][tile_x].explored = true;

                    if (dungeon->map.tiles[tile_y][tile_x].type == TILE_TYPE_WALL) {
                        wall_reached = true;
                        break;
                    }
                }
            }

            if(wall_reached) break;
        }
    }
}

void player_pickup_here(GamePlay *gameplay)
{
    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    for (int i = 0; i < dungeon->item_count; ++i) {
        Item *item = &dungeon->items[i];
        if(item->x == gameplay->player.x && item->y == gameplay->player.y){
            TraceLog(LOG_INFO,"Player got %s",item_names[item->type]);

            Entity *player = &gameplay->player;
            // add to inventory then remove from ground dummy!
            // Item it = *(Item*)item;
            inventory_add_item(&player->inventory,*item);
            dungeon_remove_item(dungeon,item->x, item->y);
        }
    }
}

void player_show_map(GamePlay *gameplay){

    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {

            dungeon->map.tiles[y][x].visible = true;
        }
    }
}
