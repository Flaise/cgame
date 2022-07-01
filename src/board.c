#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "icon.h"
#include "terrain.h"

static void make_generic_piece(State* state, Entity entity, int32_t x, int32_t y, Icon icon) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, icon) == NULL) {
        WARN("avatar_init");
    }
    if (selectable_init(&state->components, entity) == NULL) {
        WARN("selectable_init");
    }
    if (obstruction_init(&state->components, entity) == NULL) {
        WARN("obstruction_init");
    }
}

#define WALL_NORMAL 1
#define WALL_PYRAMID 2
static void make_wall(State* state, Entity entity, int32_t x, int32_t y, uint8_t walltype) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (obstruction_init(&state->components, entity) == NULL) {
        WARN("obstruction_init");
    }

    Icon icon;
    if (walltype == WALL_NORMAL) {
        icon = state->icon_wall;
    } else if (walltype == WALL_PYRAMID) {
        icon = state->icon_pyramid;
    } else {
        WARN("invalid wall type");
    }
    
    if (tile_init(&state->components, entity, icon) == NULL) {
        WARN("tile_init");
    }
}

void level_1_init(State* state) {
    /* TODO: clear entities from previous level */

    /* Pieces. */
    
    Entity entity = 1; /* TODO: function to get next entity */
    make_generic_piece(state, entity, 2, 0, state->icon_dragon);
    munch_init(&state->components, entity);
    slayme_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 4, 4, state->icon_knight);
    rider_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 4, 1, state->icon_sheep);
    edible_init(&state->components, entity);
    flock_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 2, 3, state->icon_sheep);
    edible_init(&state->components, entity);
    flock_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 4, 2, state->icon_horse);
    mount_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 7, 3, state->icon_dog);
    edible_init(&state->components, entity);
    herder_init(&state->components, entity);

    /* Terrain. */
    
    entity += 1;
    make_wall(state, entity, 0, 0, WALL_PYRAMID);
    entity += 1;
    make_wall(state, entity, 9, 0, WALL_PYRAMID);
    entity += 1;
    make_wall(state, entity, 9, 5, WALL_PYRAMID);
    entity += 1;
    make_wall(state, entity, 0, 5, WALL_PYRAMID);

    for (int32_t r = 1; r < 5; r += 1) {
        entity += 1;
        make_wall(state, entity, 0, r, WALL_NORMAL);
        entity += 1;
        make_wall(state, entity, 9, r, WALL_NORMAL);
    }
    
    for (int32_t r = 1; r < 9; r += 1) {
        entity += 1;
        make_wall(state, entity, r, 5, WALL_NORMAL);
    }
    
    for (int32_t r = 7; r < 9; r += 1) {
        entity += 1;
        make_wall(state, entity, r, 0, WALL_NORMAL);
    }
    entity += 1;
    make_wall(state, entity, 6, 0, WALL_PYRAMID);

    terrain_update(state);
}
