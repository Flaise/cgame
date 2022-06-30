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

void piece_init(State* state) {
    /* TODO: icon_texture_init for entire texture */
    state->icon_dragon = icon_tile_init(TEXTURE_DRAGON, 128, 0, 0);
    state->icon_knight = icon_tile_init(TEXTURE_KNIGHT, 128, 0, 0);
    state->icon_sheep = icon_tile_init(TEXTURE_SHEEP, 128, 0, 0);
}

void level_1_init(State* state) {
    /* TODO: clear entities from previous level */
    
    Entity entity = 1; /* TODO: function to get next entity */
    if (position_init(&state->components, entity, 2, 1) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, state->icon_dragon) == NULL) {
        WARN("avatar_init");
    }
    
    entity = 2;
    if (position_init(&state->components, entity, 4, 3) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, state->icon_knight) == NULL) {
        WARN("avatar_init");
    }
    
    entity = 3;
    if (position_init(&state->components, entity, 4, 1) == NULL) {
        WARN("position_init");
    }
    if (avatar_init(&state->components, entity, state->icon_sheep) == NULL) {
        WARN("avatar_init");
    }
}
