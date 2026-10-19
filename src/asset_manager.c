#include "asset_manager.h"

// #include "texture_helpers.h"
// #include "atlas.inl"

void set_texture_filter(AssetManager *self){
    for (int i = 0; i < 6; ++i) {
        if(self->texture_assets[i].texture.id != -1){
            // SetTextureFilter(self->texture_assets[i].texture,TEXTURE_FILTER_BILINEAR);
            SetTextureFilter(self->texture_assets[i].texture,TEXTURE_FILTER_POINT);
        }
    }
}

void update_tile_count(TextureAsset *texture_asset){
    texture_asset->tile_count_cols = (int)texture_asset->texture.width / 16;
    texture_asset->tile_count_rows = (int)texture_asset->texture.height / 16;
    texture_asset->tile_count =  texture_asset->tile_count_rows * texture_asset->tile_count_cols;

    texture_asset->tile_size_w = 16;
    texture_asset->tile_size_h = 16;
}

void asset_manager_init(AssetManager *self)
{
    memset(self,0,sizeof(AssetManager));


    // self->world.texture = NULL;
    // self->interface.texture = NULL;
    // self->characters.texture = NULL;
    // self->fx.texture = NULL;
    // self->fx_altered.texture = NULL;
    // self->items_no_shadow.texture = NULL;
}

void asset_manager_load_all(AssetManager *self)
{
    self->world.texture = LoadTexture(RESOURCES_PATH "images/tiny_dungeon_world.png");
    // self->world.texture = texture_load(renderer,RESOURCES_PATH "images/tiny_dungeon_world.png");
    update_tile_count(&self->world);

    self->interface.texture  = LoadTexture(RESOURCES_PATH "images/tiny_dungeon_interface.png");
    update_tile_count(&self->interface);

    self->characters.texture  = LoadTexture(RESOURCES_PATH "images/tiny_dungeon_monsters.png");
    update_tile_count(&self->characters);

    self->fx.texture  = LoadTexture(RESOURCES_PATH "images/tiny_dungeon_fx.png");
    update_tile_count(&self->fx);

    self->fx_altered.texture  = LoadTexture(RESOURCES_PATH "images/tiny_dungeon_fx_altered.png");
    update_tile_count(&self->fx_altered);

    self->items_no_shadow.texture  = LoadTexture(RESOURCES_PATH "images/tiny_dungeon_items_no_shadow.png");
    update_tile_count(&self->items_no_shadow);

    self->items.texture  = LoadTexture(RESOURCES_PATH "images/tiny_dungeon_items.png");
    update_tile_count(&self->items);

    //set filter
    set_texture_filter(self);
}

Rectangle asset_manager_get_texture_rect_by_index(int index, TextureAsset *texture_asset)
{

    // row    = index / width
    // column = index % width
    // index = (row * width) + column

    Rectangle rect = {};
    int tx_row = index / (texture_asset->texture.width / texture_asset->tile_size_w);
    int tx_col = index % (texture_asset->texture.width / texture_asset->tile_size_h);

    // TraceLog(LOG_INFO,"%i %i",txCol, txRow);

    rect = (Rectangle){
        .x = (float)tx_col * texture_asset->tile_size_w,
        .y = (float)tx_row *  texture_asset->tile_size_h,
        .width = (float)texture_asset->tile_size_w,
        .height = (float)texture_asset->tile_size_h
    };

    return rect;
}
