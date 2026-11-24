#include "gameplay.h"
#include "render_dungeon_system.h"
#include "render_entities.h"
#include "shared_context.h"
#include "helpers.h"
#include "inventory.h"
#include "monster.h"
#include "render_items.h"
#include "camera_system.h"
#include "factory_monster.h"
#include "ranged_mode.h"
#include "render_game_ui.h"
#include "queue_free.h"
#include "factories/factory_set_levels.h"

static void update_screen_transition(GamePlay *game, float delta_time){

    screen_transition_update(&game->transition, delta_time);

    // TraceLog(LOG_INFO,"PHASE:%u - ALPHA:%f",game->transition.phase,game->transition.alpha);

    if(!game->transition_pending) {
        return;
    }

    if(game->transition.alpha >= 1.0f){
        game->screen = game->pending_screen;
        game->transition_pending = false;
        screen_transition_begin_fade_in(&game->transition);
    }
}

static void game_change_screen(GamePlay *game, GameScreen next){

    if(game == NULL) return;
    if(game->screen == next) return;
    if(game->transition_pending) return;

    if(next == SCREEN_PLAYING){
        SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
    }else{
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    // game->screen = next;
    game->pending_screen = next;
    game->transition_pending = true;

    // game->screen = next
    screen_transition_begin_fade_out(&game->transition);
}

static void screen_shake_update(GamePlay *game, float delta_time){
    // Vector2 center = {
    //     SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f
    // };

    // game->camera.target = center;

    if(game->screen_shake.time <= 0.0f){
        game->screen_shake.intensity = 0.0f;
    }

    game->screen_shake.time -= delta_time;

    float amount = game->screen_shake.intensity;
    game->camera.target.x += (float)GetRandomValue((int)-amount,amount);
    game->camera.target.y += (float)GetRandomValue((int)-amount,amount);
}


static void change_level(GamePlay *self){

    // TraceLog(LOG_INFO,"%i",self->current_dungeon);


    // TraceLog(LOG_INFO,"level:%u is last:%b",self->current_dungeon, is_last);

    Dungeon *dungeon = gameplay_current_dungeon(self);

    // TraceLog(LOG_INFO,"current dungeon index:%i",self->current_dungeon_depth);
    if(!dungeon->generated){
        dungeon_init(dungeon,self->player.x,self->player.y, dungeon->type);
    }

    entity_set_position(&self->player,dungeon->player_start_x,dungeon->player_start_y);

    //set stairs up
    if(dungeon->type != DUNGEON_TYPE_VILLAGE){
        dungeon->stairs_up_x = dungeon->player_start_x;
        dungeon->stairs_up_y = dungeon->player_start_y;
    }else{
        dungeon->stairs_up_x = 0;
        dungeon->stairs_up_y = 0;
        // TraceLog(LOG_INFO,"NO STAIRS UP");
    }

    self->camera.target.x = self->player.x * TILE_SIZE;
    self->camera.target.y = self->player.y * TILE_SIZE;

    int dungeon_count = 0;
    for (int i = 0; i < DUNGEON_COUNT; ++i) {
        dungeon_count+=(self->dungeon[i].type != DUNGEON_TYPE_UNDEFINED)?1:0;
    }

    if((dungeon_count-1) == (self->current_dungeon_depth)){
        dungeon->stairs_down_x = 0;
        dungeon->stairs_down_y = 0;
    }

    // for rooms
    // for (int i = 0; i < dungeon->map.rooms_count; ++i) {
    // for (int i = 0; i < 1; ++i) {
    // TraceLog(LOG_INFO,"monster count:%i",dungeon->monster_count);

    if(!dungeon->generated){
        for (int m = 0; m < 1; ++m) {
            Vector2i monster_pos = dungeon_random_position_room(dungeon,0);
            factory_monster(dungeon,monster_pos.x, monster_pos.y);
            TraceLog(LOG_INFO, "monster created: %i %i",monster_pos.x, monster_pos.y);
        }
    }

    map_compute_distance_from_goal(&dungeon->map,self->player.x, self->player.y);

    dungeon->generated = true;

    // UPDATE DIFICULTY AFTERWARDS
    // int monsterCount = 8 + depth * 2;             // more, the deeper you go
    //    for (int i = 1; i < n && (int)monsters.size() < monsterCount; ++i)
    //    {
    //        // ...pick a random kind...
    //        m.hp = m.maxHp = k.hp + depth;            // hardier with depth
    //        m.power = k.power + depth / 2;            // and hits harder
    //        monsters.push_back(m);
    //    }
}

void gameplay_restart_playing(GamePlay *self)
{
    game_change_screen(self,SCREEN_PLAYING);
    // create player
    entity_init(&self->player,ENTITY_TYPE_PLAYER);

    self->current_dungeon_depth = 0;
    self->inventory = false;
    // init dungeon


    factory_set_levels(self);

    // Dungeon *dungeon = gameplay_current_dungeon(self);
    // dungeon_init(dungeon,0,0,dungeon->type);
    // dungeon_populate(dungeon,self->player.x, self->player.y,0,dungeon->type);

    // dungeon_populate(&self->dungeon[0],0,0,1,DUNGEON_TYPE_VILLAGE);
    // dungeon_populate(&self->dungeon[0],0,0,1,DUNGEON_TYPE_ROOMS);

    // self->player.sx = dungeon->player_start_x;
    // self->player.sy = dungeon->player_start_y;


    self->camera.target.x = self->player.x * TILE_SIZE;
    self->camera.target.y = self->player.y * TILE_SIZE;

    // key bindins
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_UP]          = KEY_W;
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_DOWN]        = KEY_S;
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_LEFT]        = KEY_A;
    shared_context()->input_mapper.key_bindins[ACTION_MOVE_RIGHT]       = KEY_D;
    shared_context()->input_mapper.key_bindins[ACTION_ATTACK]           = KEY_SPACE;
    shared_context()->input_mapper.key_bindins[ACTION_USE]              = KEY_E;

    self->player.hp = self->player.max_hp;

    // // for rooms
    // for (int i = 0; i < dungeon->map.rooms_count; ++i) {
    //     for (int m = 0; m < 2; ++m) {
    //         TraceLog(LOG_INFO, "monster created");
    //         Vector2i monster_pos = dungeon_random_position_room(dungeon,i);
    //         factory_monster(dungeon,monster_pos.x, monster_pos.y);
    //     }
    // }

    // for caves
    // int monster_count = 0;
    // for (int m = 0; m < monster_count; ++m) {
    //     TraceLog(LOG_INFO, "monster created");
    //     Vector2i monster_pos = dungeon_random_position_cave(dungeon);
    //     factory_monster(dungeon,monster_pos.x, monster_pos.y);
    // }


    // compute map distance
    // map_compute_distance_from_goal(&dungeon->map,self->player.x, self->player.y);
}

void gameplay_init(GamePlay *self){

    memset(self,0,sizeof(*self));

    //set camera
    self->camera.zoom = 3;
    self->camera.offset = (Vector2){(SCREEN_WIDTH*0.5f),(SCREEN_HEIGHT*0.5f)};
    // self->camera.offset = (Vector2){ GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
    self->camera.rotation = 0.0f;

    self->screen = SCREEN_MENU;
    // self->screen = SCREEN_PLAYING;

    self->transition = screen_transition_create();
    // self->transition.alpha = 0.0f;

    self->transition_counter = 0.0f;
}

void gameplay_update(GamePlay *self, float delta_time){

    if(!self) return;

    update_screen_transition(self,delta_time);

    if(self->screen == SCREEN_MENU){
        if(IsKeyPressed(KEY_ENTER)){
            // start_run(game);

            TraceLog(LOG_INFO,"START RUN!");
            // player_reset(&self->run.player);

            // gameplay_init(self);
            gameplay_restart_playing(self);
        }
        return;
    }

    // if(self->screen == SCREEN_PLAYING){
    Dungeon *dungeon = gameplay_current_dungeon(self);

    // togle inventory
    if(IsKeyPressed(KEY_I)){
        self->inventory = !self->inventory;
        self->player.inventory.selected_slot = 0;
    }

    if(IsKeyPressed(KEY_C)){
        self->dialog = !self->dialog;

        // if(self->ranged_mode.enabled){
        //     self->ranged_mode.position.x = self->player.x;
        //     self->ranged_mode.position.y = self->player.y;
        // }
    }

    // togle ranged mode
    if(IsKeyPressed(KEY_R)){
        self->ranged_mode.enabled = !self->ranged_mode.enabled;

        if(self->ranged_mode.enabled){
            self->ranged_mode.position.x = self->player.x;
            self->ranged_mode.position.y = self->player.y;
        }
    }

    bool took_turn = false;

    entity_update(&self->player, delta_time);

    if(self->inventory){
        if(inventory_update(&self->player.inventory,self)){
            took_turn = true;
        }else{
            return;
        }
    }

    int dx = 0;
    int dy = 0;

    // in ranged mode you can't move!
    if(!self->ranged_mode.enabled && self->player.state != STATE_ATTACKING){
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
    }{
        if(ranged_mode_update(self)){
            took_turn = true;
        }
    }

    entity_move_and_slide(&self->player, delta_time);
    // self->player.timer_attacking_counter += delta_time;

    for (int m = 0; m < dungeon->monster_count; ++m) {
        Entity *m_e = &dungeon->monster[m];
        entity_move_and_slide(m_e, delta_time);
        entity_update(m_e,delta_time);

        if(m_e->entity_type == ENTITY_TYPE_NPC){
            npc_update(m_e,self,delta_time);
            npc_player_in_front(m_e,&self->player);
        }
    }

    if((dx != 0 || dy != 0) || took_turn){

        // is there monster?
        // TraceLog(LOG_INFO,"MOVE!");

        Entity *target = monster_at(self, self->player.x + dx, self->player.y + dy);

        if(target && self->player.state == STATE_IDLE){

            // check if has weapon equiped
            if(!entity_has_melee_equiped(&self->player)){
                TraceLog(LOG_INFO,"player has no melee weapon equiped!");
                return;
            }
            // TraceLog(LOG_INFO,"target %p",target);
            // trigger attack
            //entity_attack(&self->player, target);
            entity_start_attacking(&self->player,target);
            // screen_shake_start(&self->screen_shake,0.08f,1.2f);
            took_turn = true;
        }else if(entity_try_move(&self->player,dx,dy,&dungeon->map)){
            // }else if(entity_try_move_smooth(&self->player,dx,dy,&dungeon->map,delta_time)){
            took_turn = true;


            // check dungeon stairs up activation (needs to be active after player left)

        }

        // self->dirty = entity_try_move(&self->player,dx,dy,&self->dungeon.map);
        // monster turn
        if(took_turn){
            gameplay_on_exit_stairs_up(self);
            player_pickup_here(self);
            monster_update(self);

            // update entity and monster status
            status_update(&self->player);

            for (int m = 0; m < dungeon->monster_count; ++m) {
                status_update(&dungeon->monster[m]);
            }

            map_compute_distance_from_goal(&dungeon->map,self->player.x, self->player.y);

            if(self->player.hp <= 0){
                self->dead = true;
                self->dirty = true;
            }
        }
    }

    camera_system(&self->camera, (Vector2){self->player.x, self->player.y}, delta_time);

    // // calculate FOV
    player_calculate_fov(self);

    gameplay_on_enter_stairs(self);
    // player_show_map(self); // jeison changes
    queue_free_system(self->dungeon);

    screen_shake_update(self,delta_time);
    // }

    // screen_transition_update(&self->transition,delta_time);
};
void gameplay_render(GamePlay *self, float delta_time){

    if(self->screen == SCREEN_PLAYING){
        Dungeon *dungeon = gameplay_current_dungeon(self);

        BeginMode2D(self->camera);

        render_dungeon(dungeon);
        // render_player(&self->player);
        render_items_system(self);
        render_entities_system(self);

        if(self->inventory){
            inventory_render(&self->player.inventory, &self->camera);
        }

        if(self->ranged_mode.enabled){
            ranged_mode_render(self);
        }

        // render_dungeon_helper(dungeon);
        render_game_ui_gameplay_system(self, delta_time);


        EndMode2D();

        render_game_ui_system_screen(self);
    }

    if(self->screen == SCREEN_MENU){

        ClearBackground((Color){10,14,24,255});

        DrawText("Fourfold: Ashes of the Primordials",245,125,34,SKYBLUE);
        DrawText("WASD to move - Mouse to aim",240,190,20,LIGHTGRAY);
        DrawText("Hold left mouse to fire",230,220,20,LIGHTGRAY);
        // DrawText(TextFormat("HIGH SCORE:%d",game->progress.high_score),300,270,20,GOLD);
        DrawText("Press ENTER to deploy",300,300,20,GOLD);
    }

    if(self->transition.alpha > 0.0f){
        DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,Fade(BLACK,self->transition.alpha));
    }
};

Dungeon *gameplay_current_dungeon(GamePlay *self)
{
    return &self->dungeon[self->current_dungeon_depth];
}

void gameplay_on_enter_stairs(GamePlay *self)
{
    // check player position and stairs

    Dungeon *dungeon = gameplay_current_dungeon(self);

    // number of dungeons
    int dungeon_count = 0;
    for (int i = 0; i < DUNGEON_COUNT; ++i) {
        dungeon_count+=(self->dungeon[i].type != DUNGEON_TYPE_UNDEFINED)?1:0;
    }

    // if(dungeon->stairs_up_active){
    //     TraceLog(LOG_INFO,"stairs up %u",true);
    // }

    bool on_stairs_up = false;
    bool on_stairs_down = false;


    if(dungeon_has_stairs_up(dungeon)){
        on_stairs_up = (self->player.x == dungeon->stairs_up_x && self->player.y == dungeon->stairs_up_y) && dungeon->stairs_up_active && dungeon->type;
    }

    if(dungeon_has_stairs_down(dungeon)){
        on_stairs_down = (self->player.x == dungeon->stairs_down_x && self->player.y == dungeon->stairs_down_y);
    }

    // TraceLog(LOG_INFO,"stairs up %u // stairs down %u ",on_stairs_up, on_stairs_down);
    // TraceLog(LOG_INFO,"player:(%i %i) // stairs:(%i %i)  can: %u",self->player.x, self->player.y,dungeon->stairs_up_x, dungeon->stairs_up_y, dungeon->stairs_up_active);

    bool on_stairs = (on_stairs_down || on_stairs_up);

    if(!on_stairs) return;

    if(self->transition.phase == TRANSITION_FADE_IN){

        if(on_stairs_down){
            self->current_dungeon_depth++;
            TraceLog(LOG_INFO,"stairs down:%i",self->current_dungeon_depth);
            Dungeon *d =gameplay_current_dungeon(self);
            d->stairs_up_active = false;
        }

        if(on_stairs_up){
            self->current_dungeon_depth--;
            TraceLog(LOG_INFO,"stairs up:%i",self->current_dungeon_depth);
        }

        TraceLog(LOG_INFO,"--");
        change_level(self);
        self->player.can_move = true;
        // self->current_dungeon++;
        return;
    }

    if(!self->transition_pending){
        self->player.can_move = false;
        self->transition_pending = true;
        screen_transition_begin_fade_out(&self->transition);
    }
}


void gameplay_on_exit_stairs_up(GamePlay *self)
{
    Dungeon *dungeon = gameplay_current_dungeon(self);
    if(dungeon->type == DUNGEON_TYPE_VILLAGE) return;

    Vector2 player = (Vector2){self->player.x,self->player.y};
    Vector2 player_next = (Vector2){dungeon->stairs_up_x,dungeon->stairs_up_y};

    int distance = (int)Vector2Distance(player,player_next);

    if(!dungeon->stairs_up_active){
        dungeon->stairs_up_active = (distance >= 1);
    }else{
        TraceLog(LOG_INFO,"stairs up active");
    }
}
