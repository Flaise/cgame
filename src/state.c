#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "constants.h"
#include "state.h"

State* make_state() {
    State* state = calloc(1, sizeof(State));
    if (state == NULL) {
        return NULL;
    }
    state->needs_redraw = true;
    state->selection.hover_x = -1;
    state->selection.hover_y = -1;
    state->selection.select_x = -1;
    state->selection.select_y = -1;
    return state;
}

void destroy_state(State* state) {
    if (state == NULL) {
        return;
    }

    for (int i = 0; i < TEXTURE_COUNT; i += 1) {
        if (state->textures[i] != NULL) {
            SDL_DestroyTexture(state->textures[i]);
        }
    }

    if (state->renderer != NULL) {
        SDL_DestroyRenderer(state->renderer);
    }
    
    if (state->window != NULL) {
        SDL_DestroyWindow(state->window);
    }

    free(state);
}
