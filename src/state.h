#include <stdbool.h>

typedef struct {
    bool needs_redraw;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* floor;
    SDL_Texture* wall;
    SDL_Texture* terrain;
} State;

/*
    Allocates and returns a new blank State. Caller must free the state. Returns NULL if the
    allocation failed.
 */
State* make_state();

/*
    Deallocates and zeroes out the state.
 */
void destroy_state(State* state);
