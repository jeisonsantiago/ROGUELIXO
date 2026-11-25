#include "render_entities.h"
#include "shared_context.h"

static void render_entity(Entity *self, AssetManager *asset_manager){

    TextureAsset *ta = &asset_manager->texture_assets[self->texture_asset_index];

    Rectangle src_floor = asset_manager_get_texture_rect_by_index(self->texture_sprite_index,ta);

    // float wave = sinf(t * 2.0f * 3.14159f);

    // Rectangle dest = {
    //     self->x * TILE_SIZE,
    //     self->y * TILE_SIZE,
    //     TILE_SIZE,
    //     TILE_SIZE
    // };

    Rectangle dest = {
        (self->visual_x * TILE_SIZE),
        (self->visual_y * TILE_SIZE),
        TILE_SIZE,
        TILE_SIZE
    };

    if(self->state == STATE_ATTACKING && self->target != NULL){

        // where is the target?
        float dx = (float)(self->target->x - self->x);
        float dy = (float)(self->target->y - self->y);

        // Reduce each component to its direction: -1, 0, or 1.
        float tx = (dx > 0) - (dx < 0);
        float ty = (dy > 0) - (dy < 0);

        // clamp it!
        float wave = Clamp(self->animation_sin_wave, 0.0f, 1.0f);
        float amplitude = TILE_SIZE * 0.1f; // 2% of a tile

        // it's just the sprite moving mate...
        dest.x += tx * wave * amplitude;
        dest.y += ty * wave * amplitude;
    }

    Color color = status_color(self);
    // Color color = WHITE;


    DrawTexturePro(ta->texture,src_floor,dest,(Vector2){0,0},0,color);

}

void render_entities_system(GamePlay *gameplay){

    AssetManager *asset_manager = &shared_context()->asset_manager;

    render_entity(&gameplay->player,asset_manager);

    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    // TraceLog(LOG_INFO,"dungeon :%i",gameplay->current_dungeon);

    for (int i = 0; i < dungeon->monster_count; ++i) {
        Entity *monster = &dungeon->monster[i];

        if(!monster->alive) continue;

        if(map_is_visible(&dungeon->map,monster->x, monster->y)){
            render_entity(monster,asset_manager);
            // DrawText(TextFormat("e:%i",i),monster->x * TILE_SIZE, monster->y * TILE_SIZE, 2,GREEN);
            // DrawLine(gameplay->player.x, gameplay->player.y, monster->x,monster->y,RED);
        }
    }

    // stairs info
    // DrawText(TextFormat("%i",dungeon->stairs_down_active),dungeon->stairs_down_x,dungeon->stairs_down_y,10,WHITE);
    // DrawText(TextFormat("%i",dungeon->stairs_up_active),dungeon->stairs_up_x,dungeon->stairs_up_y,10,RED);

    // // render distance map number
    // for (int x = 0; x < MAP_WIDTH; ++x) {
    //     for (int y = 0; y < MAP_HEIGHT; ++y) {

    //         if(dungeon->map.tiles[y][x].type == TILE_TYPE_WALL) continue;


    //         Tile *tile = &dungeon->map.tiles[y][x];

    //         Rectangle dest = {
    //             x * TILE_SIZE+ 8,
    //             y * TILE_SIZE+ 8,
    //             TILE_SIZE,
    //             TILE_SIZE
    //         };

    //         DrawText(TextFormat("%u",dungeon->map.dist[y][x]),dest.x, dest.y, 2,GREEN);

    //     }
    // }

    // render distance map number
    // for (int x = 0; x < MAP_WIDTH; ++x) {
    //     for (int y = 0; y < MAP_HEIGHT; ++y) {

    //         Rectangle dest = {
    //             x * TILE_SIZE,
    //             y * TILE_SIZE,
    //             TILE_SIZE,
    //             TILE_SIZE
    //         };


    //         DrawTextEx(GetFontDefault(),TextFormat("(%i %i)",x,y),(Vector2){dest.x, dest.y}, 6,1,GREEN);

    //     }
    // }
}