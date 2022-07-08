#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "icon.h"
#include "terrain.h"
#include "draw.h"

#define LEVEL_MAX 3

bool in_board(Coord tile_x, Coord tile_y) {
    return tile_x >= 0 && tile_y >= 0 && tile_x < TILES_ACROSS && tile_y < TILES_DOWN;
}

static void make_generic_piece(State* state, Entity entity, int32_t x, int32_t y, IconID icon_id) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, icon_id, (float_t)x, (float_t)y) == NULL) {
        WARN("avatar_init");
    }
    if (selectable_init(&state->components, entity) == NULL) {
        WARN("selectable_init");
    }
    if (obstruction_init(&state->components, entity) == NULL) {
        WARN("obstruction_init");
    }
    if (tween_init(&state->components, entity) == NULL) {
        WARN("tween_init");
    }
}

static void make_sheep(State* state, Entity entity, int32_t x, int32_t y) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, ICON_SHEEP, (float_t)x, (float_t)y) == NULL) {
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
    if (tween_init(&state->components, entity) == NULL) {
        WARN("tween_init");
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

static void level_2_init(State* state) {
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
}

static void level_1_init(State* state) {
    /* Pieces. */
    
    Entity entity = 1; /* TODO: function to get next entity */
    make_generic_piece(state, entity, 3, 2, ICON_DRAGON);
    munch_init(&state->components, entity);
    slayme_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 6, 3, ICON_KNIGHT);
    rider_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 5, 3, ICON_HORSE);
    mount_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_sheep(state, entity, 7, 2);
    
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
        
        entity += 1;
        make_wall(state, entity, 1, r, ICON_WALL);
        entity += 1;
        make_wall(state, entity, 8, r, ICON_WALL);
    }
    
    for (int32_t r = 1; r < 9; r += 1) {
        entity += 1;
        make_wall(state, entity, r, 5, ICON_WALL);
        entity += 1;
        make_wall(state, entity, r, 0, ICON_WALL);
    }
    
    for (int32_t r = 2; r < 8; r += 1) {
        entity += 1;
        make_wall(state, entity, r, 4, ICON_WALL);
        entity += 1;
        make_wall(state, entity, r, 1, ICON_WALL);
    }
}

static void level_3_init(State* state) {
    /* Pieces. */
    
    Entity entity = 1; /* TODO: function to get next entity */
    make_generic_piece(state, entity, 4, 3, ICON_DRAGON);
    munch_init(&state->components, entity);
    slayme_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 3, 4, ICON_KNIGHT);
    rider_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_generic_piece(state, entity, 5, 4, ICON_HORSE);
    mount_init(&state->components, entity);
    edible_init(&state->components, entity);
    
    entity += 1;
    make_sheep(state, entity, 3, 1);
    entity += 1;
    make_sheep(state, entity, 5, 3);
    entity += 1;
    make_sheep(state, entity, 6, 1);
    entity += 1;
    make_sheep(state, entity, 1, 2);
    
    entity += 1;
    make_generic_piece(state, entity, 5, 1, ICON_DOG);
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

    
    entity += 1;
    make_wall(state, entity, 6, 3, ICON_PYRAMID);
    entity += 1;
    make_wall(state, entity, 6, 4, ICON_WALL);

    for (int32_t r = 1; r < 5; r += 1) {
        entity += 1;
        make_wall(state, entity, 0, r, ICON_WALL);
        entity += 1;
        make_wall(state, entity, 9, r, ICON_WALL);
    }
    
    for (int32_t r = 1; r < 9; r += 1) {
        entity += 1;
        make_wall(state, entity, r, 5, ICON_WALL);
        entity += 1;
        make_wall(state, entity, r, 0, ICON_WALL);
    }
}

static void level_id_init(State* state, LevelID level_id) {
    components_clear(&state->components);
    state->level_id = level_id;
    state->game_over = false;
    state->won = false;
    
    if (level_id == 1) {
        level_1_init(state);
    } else if (level_id == 2) {
        level_2_init(state);
    } else if (level_id == 3) {
        level_3_init(state);
    } else {
        WARN("Invalid level_id %d.", level_id);
        return;
    }

    terrain_update(state);
    redraw(state);
}

void level_init(State* state) {
    level_id_init(state, 1);
}

void level_restart(State* state) {
    level_id_init(state, state->level_id);
}

static LevelID wrap_level(LevelID level_id) {
    if (level_id <= 0) {
        return LEVEL_MAX;
    }
    if (level_id > LEVEL_MAX) {
        return 1;
    }
    return level_id;
}

void level_next(State* state) {
    level_id_init(state, wrap_level(state->level_id + 1));
}

void level_prev(State* state) {
    level_id_init(state, wrap_level(state->level_id - 1));
}
