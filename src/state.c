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
    state->floor = NULL;
    return state;
}

void destroy_state(State* state) {
    if (state == NULL) {
        return;
    }

    if (state->floor != NULL) {
        SDL_FreeSurface(state->floor);
        state->floor = NULL;
    }
    
    if (state->window != NULL) {
        SDL_DestroyWindow(state->window);
        state->window = NULL;
    }
}
