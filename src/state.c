#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "state.h"
#include "audio.h"
#include "draw.h"

State* state_new() {
    /* Using calloc to initialize to zero. */
    State* state = calloc(1, sizeof(State));
    if (state == NULL) {
        ERROR("calloc");
        return NULL;
    }
    state->needs_redraw = true;
    state->selection.hover_x = -1;
    state->selection.hover_y = -1;
    state->selection.select_x = -1;
    state->selection.select_y = -1;
    state->components = components_new();
    return state;
}

void state_end(State* state) {
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

    draw_loading_done();
    
    audio_done_blocking(state);

    free(state);
}
