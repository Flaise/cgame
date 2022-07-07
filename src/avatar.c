#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "icon.h"

static void avatar_draw_one(State* state, CAvatar* avatar) {
    SDL_Rect dest_rect = {
        .x = (int32_t)(TILE_SIZE * avatar->x),
        .y = (int32_t)(TILE_SIZE * avatar->y),
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };
    icon_draw(state, avatar->icon_id, &dest_rect);
}

void avatar_draw(State* state) {
    CompGroup* groups[] = {
        &state->components.compgroups[COMPTYPE_AVATAR],
    };
    void* comps[] = {NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 1)) {
        CAvatar* avatar = (CAvatar*)comps[0];

        avatar_draw_one(state, avatar);
    }
}
