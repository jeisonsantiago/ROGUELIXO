#include "render_dungeon_system.h"
#include "shared_context.h"

#define NG_4 4
#define NG_8 8

void render_dungeon(Dungeon *dungeon){

    AssetManager *asset_manager = &shared_context()->asset_manager;

    TextureAsset *ta = &asset_manager->texture_assets[0];

    Rectangle src_floor = asset_manager_get_texture_rect_by_index(dungeon->tile_index_floor,ta);
    Rectangle src_wall = asset_manager_get_texture_rect_by_index(dungeon->tile_index_wall,ta);
    Rectangle src_wall_out = asset_manager_get_texture_rect_by_index(3,ta);
    Rectangle src_wall_side = asset_manager_get_texture_rect_by_index(6,ta);

    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {

            Tile *tile = &dungeon->map.tiles[y][x];

            Color color = tile->visible ? WHITE : BLACK;
            if(tile->explored &&!tile->visible) color = ColorAlpha(GRAY,0.5f);

            Rectangle dest = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            switch (tile->type) {
            case TILE_TYPE_FLOOR:{
                DrawTexturePro(
                            ta->texture,
                            src_floor,dest,(Vector2){0,0},0,color);
                break;
            }
            case TILE_TYPE_WALL:{
                DrawTexturePro(
                            ta->texture,
                            src_wall,dest,(Vector2){0,0},0,color);
                break;
            }
            default:
                break;
            }
        }
    }

    // check neighbours
    // x y
    int ng_4[NG_4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
    int ng_8[NG_8][2] = {{0,-1},{1,0},{0,1},{-1,0},{-1,-1},{1,-1},{1,1},{-1,1}};
    int ng_config[NG_8] = {1,4,16,64,128,2,8,32};





    // return;
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {

            if(dungeon->map.tiles[y][x].type != TILE_TYPE_WALL) continue;


            Tile *tile = &dungeon->map.tiles[y][x];

            Rectangle dest = {
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };

            int sum_ng = 0;
            for (int n = 0; n < NG_8; ++n) {
                int next_ng_x = x + ng_8[n][0];
                int next_ng_y = y + ng_8[n][1];

                // check if is inside
                if(
                        map_is_inside(&dungeon->map,next_ng_x,next_ng_y) &&
                        dungeon->map.tiles[next_ng_y][next_ng_x].type == TILE_TYPE_FLOOR
                        )
                {
                    sum_ng+=ng_config[n];
                }
            }
            if(sum_ng > 0){
                Rectangle src_wall = src_wall_side;



                if(
                    sum_ng == 56 ||
                    sum_ng == 131 ||
                    sum_ng == 48 ||
                    sum_ng == 24 ||
                    sum_ng == 3 ||
                    sum_ng == 129 ||
                    sum_ng == 2 ||
                    sum_ng == 128 ||
                    sum_ng == (32+16+8+4+2) ||
                    sum_ng == (8+16+32+64+128) ||
                    sum_ng == (128+64+32+16+8+4+2) ||
                    sum_ng == (1+2+4+8+16+32+64+128 ) ||
                    sum_ng == (128+1+2+4+8+16+32) ||
                    sum_ng == (8+16+32+64+128+1+2) ||
                    sum_ng == (16+32+64+128) ||
                    sum_ng == (128+64+32+16) ||
                    sum_ng == (8+16+32+128+1+2) ||
                    sum_ng == (64+32+16+8) ||
                    sum_ng == (4+8+16+32) ||
                    sum_ng == (64+32+16+8+4+2) ||
                    sum_ng == (4+8+16+32+64+128) ||
                    sum_ng == (1+2+32+16+8) ||
                    sum_ng == (1+128+32+16+8) ||
                    sum_ng == (128+1+2+16+8) ||
                    sum_ng == (128+1+2+16+32) ||
                    sum_ng == (2+32+16+8) ||
                    sum_ng == (128+32+16+8) ||
                    sum_ng == (1+2+32+16) || // weird case
                    sum_ng == (1+128+64+32+16+8) ||
                    sum_ng == (128+1+2+64+32+16) ||
                    sum_ng == (128+1+2+4+8+16) ||
                    sum_ng == (8+16+32+64+128) ||
                    sum_ng == (16+8+4+2)
                        ){
                    src_wall = src_wall_out;
                }


                // tile->type = TILE_TYPE_WALL;

                Color color = (tile->visible)?WHITE:BLACK;
                if(tile->explored &&!tile->visible) color = ColorAlpha(GRAY,0.5f);

                DrawTexturePro(
                            ta->texture,
                            src_wall,dest,(Vector2){0,0},0,color);
            }
        }
    }


}