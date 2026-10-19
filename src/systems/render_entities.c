#include "render_entities.h"
#include "shared_context.h"

static void render_entity(Entity *self, AssetManager *asset_manager){

    TextureAsset *ta = &asset_manager->texture_assets[self->texture_asset_index];

    Rectangle src_floor = asset_manager_get_texture_rect_by_index(self->texture_sprite_index,ta);

    Rectangle dest = {
        self->x * TILE_SIZE,
        self->y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
    };

    Color color = status_color(self);

    DrawTexturePro(ta->texture,src_floor,dest,(Vector2){0,0},0,color);
}

void render_entities_system(GamePlay *gameplay){

    AssetManager *asset_manager = &shared_context()->asset_manager;

    render_entity(&gameplay->player,asset_manager);

    for (int i = 0; i < gameplay->dungeon.monster_count; ++i) {
        Entity *monster = &gameplay->dungeon.monster[i];

        if(!gameplay->dungeon.monster->alive) return;

        if(entity_distance(&gameplay->player, monster) < gameplay->player.data.player.fov_distance){
            render_entity(monster,asset_manager);
        }

    }

    // render distance map number
    // for (int x = 0; x < MAP_WIDTH; ++x) {
    //     for (int y = 0; y < MAP_HEIGHT; ++y) {

    //         if(gameplay->dungeon.map.tiles[y][x].type == TILE_TYPE_WALL) continue;


    //         Tile *tile = &gameplay->dungeon.map.tiles[y][x];

    //         Rectangle dest = {
    //             x * TILE_SIZE+ 8,
    //             y * TILE_SIZE+ 8,
    //             TILE_SIZE,
    //             TILE_SIZE
    //         };

    //         DrawText(TextFormat("%u",gameplay->dungeon.map.dist[y][x]),dest.x, dest.y, 2,GREEN);

    //     }
    // }
}