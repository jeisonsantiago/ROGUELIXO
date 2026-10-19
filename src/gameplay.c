#include "gameplay.h"
#include "render_dungeon_system.h"
#include "render_entities.h"
#include "shared_context.h"
#include "helpers.h"
#include "inventory.h"
#include "monster.h"
#include "render_items.h"
#include "camera_system.h"

void gameplay_init(GamePlay *self){
    // create player
    entity_init(&self->player,ENTITY_TYPE_PLAYER);

    // init dungeon
    dungeon_init(&self->dungeon, self->player.x, self->player.y);

    // key bindins
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_UP]          = KEY_W;
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_DOWN]        = KEY_S;
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_LEFT]        = KEY_A;
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_RIGHT]       = KEY_D;
    shared_context()->input_mapper.key_bindins[ACTION_ATTACK]           = KEY_SPACE;
    shared_context()->input_mapper.key_bindins[ACTION_USE]              = KEY_E;

    // set player position inside first room;
    self->player.x = centre_x(&self->dungeon.map.rooms[0]);
    self->player.y = centre_y(&self->dungeon.map.rooms[0]);
    self->player.hp = self->player.max_hp;

    // set monster
    int monster_index = 0;
    entity_init(&self->dungeon.monster[monster_index],ENTITY_TYPE_MONSTER);
    self->dungeon.monster[monster_index].x = centre_x(&self->dungeon.map.rooms[0]) + 1;
    self->dungeon.monster[monster_index].y = centre_y(&self->dungeon.map.rooms[0]) + 1;
    self->dungeon.monster->trait.type = TRAIT_VENOMOUS;
    self->dungeon.monster->trait.turns = 2;
    self->dungeon.monster->trait.chance = 25; // 25% chance of proc

    self->dungeon.monster_count++;

    // compute map distance
    map_compute_distance_from_goal(&self->dungeon.map,self->player.x, self->player.y);

    //set camera
    self->camera.zoom = 3;
    self->camera.offset = (Vector2){(SCREEN_WIDTH*0.5f),(SCREEN_HEIGHT*0.5f)};
    // self->camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
    self->camera.rotation = 0.0f;

    self->camera.target.x = self->player.x * TILE_SIZE;
    self->camera.target.y = self->player.y * TILE_SIZE;

    self->game_state = GAME_STATE_PLAYING;
}

void gameplay_update(GamePlay *self, float delta_time){


    // movement
    // move player by input
    // player->physics.direction.x = 0.0f;
    // player->physics.direction.y = 0.0f;

    if(self->game_state == GAME_STATE_PLAYING){


        // togle inventory
        if(IsKeyPressed(KEY_I)){
            self->inventory = !self->inventory;
            self->player.inventory.selected_slot = 0;
        }

        bool took_turn = false;

        if(self->inventory){
            if(inventory_update(&self->player.inventory,self)){
                took_turn = true;
            }else{
                return;
            }
        }

        int dx = 0;
        int dy = 0;

        if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_UP]){
            dy = -1;
        }
        if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_DOWN]){
            dy = +1;
        }
        if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_LEFT]){
            dx = -1;
        }
        if(shared_context()->input_state.time_out_actions_pressed[ACTION_MOVE_RIGHT]){
            dx = +1;
        }

        if((dx != 0 || dy != 0) || took_turn){

            // is there monster?


            Entity *target = monster_at(self, self->player.x + dx, self->player.y + dy);

            if(target){
                entity_attack(&self->player, target);
                took_turn = true;
            }else if(entity_try_move(&self->player,dx,dy,&self->dungeon.map)){
                took_turn = true;
            }

            // self->dirty = entity_try_move(&self->player,dx,dy,&self->dungeon.map);
            // monster turn
            if(took_turn){
                player_pickup_here(self);
                monster_update(self);

                // update entity and monster status
                status_update(&self->player);
                status_update(&self->dungeon.monster[0]);

                map_compute_distance_from_goal(&self->dungeon.map,self->player.x, self->player.y);
                if(self->player.hp <= 0){
                    self->dead = true;
                    self->dirty = true;
                }
            }
        }

        camera_system(&self->camera, (Vector2){self->player.x, self->player.y}, delta_time);

        // // calculate FOV
        player_calculate_fov(self);
    }
};
void gameplay_render(GamePlay *self, float delta_time){

    BeginMode2D(self->camera);

    render_dungeon(&self->dungeon);
    // render_player(&self->player);
    render_items_system(self);
    render_entities_system(self);

    if(self->inventory){
        inventory_render(&self->player.inventory, &self->camera);
    }

    EndMode2D();
};
