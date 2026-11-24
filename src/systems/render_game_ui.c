#include "render_game_ui.h"
#include "gameplay.h"
#include "asset_manager.h"
#include "shared_context.h"


static void render_health_bar(Entity *entity){
    Rectangle rect_board = {entity->visual_x * TILE_SIZE, entity->visual_y * TILE_SIZE -2, TILE_SIZE, 4};
    DrawRectanglePro(rect_board,(Vector2){0},0,BLACK);

    Rectangle rect_health = {rect_board.x+1, rect_board.y+1, rect_board.width-2, rect_board.height-2};
    DrawRectanglePro(rect_health,(Vector2){0},0,BLACK);
    float res = ((float)entity->hp / (float)entity->max_hp);
    rect_health.width *= res;
    DrawRectanglePro(rect_health,(Vector2){0},0,RED);
}

static void render_player_health_bar(Entity *entity, int x, int y){
    Rectangle rect_board = {
        x * TILE_SIZE,
        y * TILE_SIZE -2,
        TILE_SIZE*16, 16};
    DrawRectanglePro(rect_board,(Vector2){0},0,BLACK);

    Rectangle rect_health = {rect_board.x, rect_board.y, rect_board.width-2, rect_board.height-2};
    DrawRectanglePro(rect_health,(Vector2){0},0,DARKGRAY);

    float res = ((float)entity->hp / (float)entity->max_hp);
    rect_health.width *= res;
    DrawRectanglePro(rect_health,(Vector2){0},0,RED);

    char hp[20];
    strcpy(hp,TextFormat("%i/%i",entity->hp,entity->max_hp));
    DrawText(hp,rect_board.x+rect_board.width+2,rect_board.y,20,WHITE);
}


static void render_npc_info(Entity *entity){

    AssetManager *asset_manager = &shared_context()->asset_manager;
    TextureAsset *ta = &asset_manager->interface;

    Rectangle exclamation = asset_manager_get_texture_rect_by_index(106,ta);

    Rectangle dest = {
        entity->x * TILE_SIZE,
        (entity->y-1) * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE
    };

    DrawTexturePro(ta->texture,exclamation,dest,(Vector2){0,0},0,WHITE);
}

static void render_npc_dialog(Entity *entity, Camera2D *camera, float delta_time){

    int w = SCREEN_WIDTH;
    int h = SCREEN_HEIGHT;

    // center
    int dialog_w = 300;
    int dialog_h = 80;

    Vector2 s2w = GetScreenToWorld2D((Vector2){w*0.5f,h*0.8f},*camera); // center
    Rectangle dest = (Rectangle){
            s2w.x - (dialog_w * 0.5f),
            s2w.y - (dialog_h * 0.5f),
            dialog_w,
            dialog_h
            };

    Rectangle ref = dest;
    DrawRectangleRec(dest,DARKGRAY);
    DrawRectangleLinesEx(dest,1,GRAY);

    NPC *npc = &entity->data.npc;
    DialogParagraph *paragraph = &npc->paragraph[npc->current_paragraph];

    if(npc->paragraph[npc->current_paragraph].show_paragraph){
        DrawText(TextSubtext(npc->dialog,paragraph->start,paragraph->end),dest.x+2, dest.y+2,5,WHITE);
    }else{
        DrawText(TextSubtext(npc->dialog,paragraph->start,(npc->dialog_frame_counter/2)<paragraph->end?npc->dialog_frame_counter/2:paragraph->end),dest.x+2, dest.y+2,5,WHITE);
    }
}

void render_game_ui_gameplay_system(GamePlay *gameplay, float delta_time){

    if(gameplay->inventory) return;



    Dungeon *dungeon = gameplay_current_dungeon(gameplay);
    // render monster health
    for (int i = 0; i < dungeon->monster_count; ++i) {
        Entity *actor = &dungeon->monster[i];

        if(actor->entity_type == ENTITY_TYPE_NPC) {
            if(npc_player_in_front(actor,&gameplay->player) &&
                    actor->data.npc.has_dialog && !actor->data.npc.is_talking){
                // TraceLog(LOG_INFO,"FRONT!");
                render_npc_info(actor);


                // TraceLog(LOG_INFO,"%s",actor->data.npc.dialog);

            }
            if(actor->data.npc.is_talking){
                render_npc_dialog(actor, &gameplay->camera, delta_time);
            }
        }

        if(map_is_visible(&dungeon->map,actor->x, actor->y) && actor->alive){

            if(actor->entity_type==ENTITY_TYPE_MONSTER){
                render_health_bar(actor);
            }
        }
    }
}

void render_game_ui_system_screen(GamePlay *gameplay){
    // render player health using screen coordinates
    render_player_health_bar(&gameplay->player,1,1);
}