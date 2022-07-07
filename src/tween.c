#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"

void tween_update(State* state) {
    CompGroup* groups[] = {
        &state->components.compgroups[COMPTYPE_AVATAR],
        &state->components.compgroups[COMPTYPE_POSITION],
        &state->components.compgroups[COMPTYPE_TWEEN],
    };
    void* comps[] = {NULL, NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 3)) {
        CAvatar* avatar = (CAvatar*)comps[0];
        CPosition* position = (CPosition*)comps[1];

        float_t factor = 4.0f;

        /* TODO: This won't work properly in an unstable framerate. */
        avatar->x += (((float_t)position->x) - avatar->x) / factor;
        avatar->y += (((float_t)position->y) - avatar->y) / factor;
    }
}
