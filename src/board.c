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
#include "draw.h"

static void make_generic_piece(State* state, Entity entity, int32_t x, int32_t y, IconID icon_id) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, icon_id) == NULL) {
        WARN("avatar_init");
    }
    if (selectable_init(&state->components, entity) == NULL) {
        WARN("selectable_init");
    }
    if (obstruction_init(&state->components, entity) == NULL) {
        WARN("obstruction_init");
    }
}

static void make_sheep(State* state, Entity entity, int32_t x, int32_t y) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, ICON_SHEEP) == NULL) {
        WARN("avatar_init");
    }
    if (obstruction_init(&state->components, entity) == NULL) {
        WARN("obstruction_init");
    }
    if (edible_init(&state->components, entity) == NULL) {
        WARN("edible_init");
    }
    if (flock_init(&state->components, entity) == NULL) {
        WARN("flock_init");
    }
}

static void make_wall(State* state, Entity entity, int32_t x, int32_t y, IconID icon_id) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (obstruction_init(&state->components, entity) == NULL) {
        WARN("obstruction_init");
    }
    if (tile_init(&state->components, entity, icon_id) == NULL) {
        WARN("tile_init");
    }
}

void level_1_init(State* state) {
    components_clear(&state->components);

    /* Pieces. */
    
    Entity entity = 1; /* TODO: function to get next entity */
    make_generic_piece(state, entity, 2, 0, ICON_DRAGON);
    munch_init(&state->components, entity);
    slayme_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 4, 4, ICON_KNIGHT);
    rider_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_sheep(state, entity, 4, 1);
    
    entity += 1;
    make_sheep(state, entity, 2, 3);
    
    entity += 1;
    make_sheep(state, entity, 5, 0);
    
    entity += 1;
    make_generic_piece(state, entity, 4, 2, ICON_HORSE);
    mount_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 7, 3, ICON_DOG);
    edible_init(&state->components, entity);
    herder_init(&state->components, entity);

    /* Terrain. */
    
    entity += 1;
    make_wall(state, entity, 0, 0, ICON_PYRAMID);
    entity += 1;
    make_wall(state, entity, 9, 0, ICON_PYRAMID);
    entity += 1;
    make_wall(state, entity, 9, 5, ICON_PYRAMID);
    entity += 1;
    make_wall(state, entity, 0, 5, ICON_PYRAMID);

    for (int32_t r = 1; r < 5; r += 1) {
        entity += 1;
        make_wall(state, entity, 0, r, ICON_WALL);
        entity += 1;
        make_wall(state, entity, 9, r, ICON_WALL);
    }
    
    for (int32_t r = 1; r < 9; r += 1) {
        entity += 1;
        make_wall(state, entity, r, 5, ICON_WALL);
    }
    
    for (int32_t r = 7; r < 9; r += 1) {
        entity += 1;
        make_wall(state, entity, r, 0, ICON_WALL);
    }
    entity += 1;
    make_wall(state, entity, 6, 0, ICON_PYRAMID);
    
    entity += 1;
    make_wall(state, entity, 1, 3, ICON_PYRAMID);
    entity += 1;
    make_wall(state, entity, 1, 4, ICON_WALL);

    terrain_update(state);
    redraw(state);
}
