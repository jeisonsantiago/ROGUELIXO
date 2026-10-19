#ifndef SHARED_CONTEXT_H
#define SHARED_CONTEXT_H

#include "asset_manager.h"
// #include "game_definitions.h"
// #include "event_system.h"
#include "input_state.h"

typedef struct{
    AssetManager asset_manager;
    InputMapper input_mapper;
    InputState input_state;
    // Vector2 mouse_position;
    // Vector2 virtual_mouse_position;
    // Vector2i world_tile_mouse_position;
}SharedContext;

SharedContext *shared_context(void);


#endif // SHARED_CONTEXT_H
