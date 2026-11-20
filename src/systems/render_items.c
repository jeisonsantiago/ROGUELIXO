#include "render_items.h"
#include "shared_context.h"

#include "gameplay.h"

static void render_item(Item *self, AssetManager *asset_manager){

    TextureAsset *ta = &asset_manager->items;
    Rectangle src = asset_manager_get_texture_rect_by_index(self->sprite_index,ta);

    Rectangle dest = {
        self->x * TILE_SIZE,
        self->y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
    };

    DrawTexturePro(ta->texture,src,dest,(Vector2){0,0},0,WHITE);
}

void render_items_system(GamePlay *gameplay){

    AssetManager *asset_manager = &shared_context()->asset_manager;

    // render_entity(&gameplay->player,asset_manager);
    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    for (int i = 0; i < dungeon->item_count; ++i) {

        Item *item = &dungeon->items[i];

        if(dungeon->map.tiles[item->y][item->x].visible){
            render_item(&dungeon->items[i],asset_manager);
        }
    }
}