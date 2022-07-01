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

void make_generic_piece(State* state, Entity entity, int32_t x, int32_t y, Icon icon) {
    if (position_init(&state->components, entity, x, y) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, icon) == NULL) {
        WARN("avatar_init");
    }
    if (selectable_init(&state->components, entity) == NULL) {
        WARN("selectable_init");
    }
}

void level_1_init(State* state) {
    /* TODO: clear entities from previous level */
    
    Entity entity = 1; /* TODO: function to get next entity */
    make_generic_piece(state, entity, 2, 1, state->icon_dragon);
    
    entity = 2;
    make_generic_piece(state, entity, 4, 3, state->icon_knight);
    rider_init(&state->components, entity);
    
    entity = 3;
    make_generic_piece(state, entity, 4, 1, state->icon_sheep);
    
    entity = 4;
    make_generic_piece(state, entity, 4, 2, state->icon_horse);
    mount_init(&state->components, entity);
    
    entity = 5;
    make_generic_piece(state, entity, 7, 3, state->icon_dog);
}
