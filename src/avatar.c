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

static void avatar_draw_one(State* state, CAvatar* avatar, CPosition* position) {
    SDL_Rect dest_rect = {
        .x = TILE_SIZE * position->x,
        .y = TILE_SIZE * position->y,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };
    icon_draw(state, avatar->icon_id, &dest_rect);
}

void avatar_draw(State* state) {
    CompGroup* groups[] = {
        &state->components.compgroups[COMPTYPE_AVATAR],
        &state->components.compgroups[COMPTYPE_POSITION],
    };
    void* comps[] = {NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 2)) {
        CAvatar* avatar = (CAvatar*)comps[0];
        CPosition* position = (CPosition*)comps[1];

        avatar_draw_one(state, avatar, position);
    }
}
