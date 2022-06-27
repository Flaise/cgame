#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "state.h"

State* make_state() {
    State* state = malloc(sizeof(State));
    if (state == NULL) {
        return NULL;
    }
    state->needs_redraw = true;
    state->window = NULL;
    state->tiles = NULL;
    state->hover_x = -1;
    state->hover_y = -1;
    return state;
}

void destroy_state(State* state) {
    if (state == NULL) {
        return;
    }
    
    if (state->tiles != NULL) {
        SDL_DestroyTexture(state->tiles);
        state->tiles = NULL;
    }
    
    if (state->terrain != NULL) {
        SDL_DestroyTexture(state->terrain);
        state->terrain = NULL;
    }

    if (state->renderer != NULL) {
        SDL_DestroyRenderer(state->renderer);
        state->renderer = NULL;
    }
    
    if (state->window != NULL) {
        SDL_DestroyWindow(state->window);
        state->window = NULL;
    }
}
