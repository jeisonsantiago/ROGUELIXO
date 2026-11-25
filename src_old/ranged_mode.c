#include "ranged_mode.h"

#include "gameplay.h"
#include "shared_context.h"

bool ranged_mode_update(GamePlay *gameplay){
    int dx = 0;
    int dy = 0;

    RangedMode *ranged_mode = &gameplay->ranged_mode;
    Entity *player = &gameplay->player;

    if(player->state == STATE_ATTACKING) return false;

    // in ranged mode you can't move!
    if(gameplay->ranged_mode.enabled){
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
    }

    gameplay->ranged_mode.position.x += dx;
    gameplay->ranged_mode.position.y += dy;

    Dungeon *dungeon = gameplay_current_dungeon(gameplay);

    // check position
    Entity *monster = monster_at(gameplay,ranged_mode->position.x, ranged_mode->position.y);
    ranged_mode->can_shoot = !(ranged_mode->position.x == player->x && ranged_mode->position.y == player->y ) && (monster != NULL);

    if(ranged_mode->enabled && ranged_mode->can_shoot){
        if(shared_context()->input_state.time_out_actions_pressed[ACTION_USE]){

            // check if is ranged weapon?!?!?!
            Item *item = &player->inventory.inventory[player->weapon].item;

            if(player->has_weapon && item->type == ITEM_TYPE_WEAPON && item->data.weapon.type == WEAPON_RANGED){
                TraceLog(LOG_INFO,"ATTACK RANGED!");

                if(map_is_visible(&dungeon->map,monster->x, monster->y)){
                    //entity_attack(&gameplay->player, monster);
                    entity_start_attacking(&gameplay->player, monster);
                    return true;
                }
            }else{
                TraceLog(LOG_INFO,"NO RANGED WEAPON EQUIPED!");
                return false;
            }
        }
    }

    return false;
}

void ranged_mode_render(GamePlay *gameplay){
    if(gameplay->ranged_mode.enabled){

        Color color = (gameplay->ranged_mode.can_shoot)?GREEN:RED;

        Rectangle rect = {
            gameplay->ranged_mode.position.x * TILE_SIZE,
            gameplay->ranged_mode.position.y * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE
        };

        // DrawRectangleRoundedLines(rect,0.01f,4,color);
        DrawRectangleLinesEx(rect,1,color);
    }
}