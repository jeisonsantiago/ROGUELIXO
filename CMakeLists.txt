cmake_minimum_required(VERSION 3.16)

project(ROGUELIXO LANGUAGES C)

# set production flags
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

include_directories(
    src
    src/systems
    src/factories
)

add_executable("${CMAKE_PROJECT_NAME}" src/main.c
    src/pch.h
    src/map.h src/map.c
    src/room.h src/room.c
    src/helpers.h src/helpers.c
    src/asset_manager.h src/asset_manager.c
    src/dungeon.h src/dungeon.c
    src/systems/render_dungeon_system.h src/systems/render_dungeon_system.c
    src/shared_context.h src/shared_context.c
    src/gameplay.h src/gameplay.c
    src/player.h src/player.c
    src/input_state.h src/input_state.c
    src/entity.h src/entity.c
    src/monster.h src/monster.c
    src/systems/render_entities.h src/systems/render_entities.c
    src/item.h src/item.c
    src/systems/render_items.h src/systems/render_items.c
    src/systems/camera_system.h src/systems/camera_system.c
    src/inventory.h src/inventory.c
    src/entity_status.h src/entity_status.c
    src/trait.h src/trait.c
    src/factories/factory_monster.h src/factories/factory_monster.c
    src/factories/factory_item.h src/factories/factory_item.c
    src/ranged_mode.h src/ranged_mode.c
    src/systems/render_game_ui.h src/systems/render_game_ui.c
    src/systems/queue_free.h src/systems/queue_free.c
    src/screen.h src/screen.c
    src/factories/factory_npc.h src/factories/factory_npc.c
    src/npc.h src/npc.c
    src/factories/factory_set_levels.h src/factories/factory_set_levels.c
)

if(PRODUCTION_BUILD)
        # setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")

	# remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)

else()
        # This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)

endif()

if(UNIX)
    # add raylib
    find_library(LIB_RAYLIB raylib ${CMAKE_SOURCE_DIR}/vendor/linux/raylib)

    if(NOT LIB_RAYLIB)
	message(${CMAKE_SOURCE_DIR}/vendor/linux/raylib)
	message(FATAL_ERROR "raylib library not found")
    endif()

    # ----- INCLUDES ----
    include_directories(
	${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/linux/raylib/include
    )

    target_link_libraries(${PROJECT_NAME} ${LIB_RAYLIB} m X11)
endif()

target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)


# Fourfold: Ashes of the Primordials