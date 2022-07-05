#include "SDL.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "icon.h"

static void cooldown_draw_one(State* state, CPosition* position) {
    SDL_Rect dest_rect = {
        .x = TILE_SIZE * position->x,
        .y = TILE_SIZE * position->y,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };
    icon_draw(state, ICON_COOLDOWN, &dest_rect);
}

void cooldown_draw(State* state) {
    CompGroup* groups[] = {
        &state->components.compgroups[COMPTYPE_COOLDOWN],
        &state->components.compgroups[COMPTYPE_POSITION],
    };
    void* comps[] = {NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 2)) {
        CPosition* position = (CPosition*)comps[1];

        cooldown_draw_one(state, position);
    }
}
