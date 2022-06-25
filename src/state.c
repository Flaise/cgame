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
    state->window = NULL;
    state->screen = NULL;
    return state;
}
