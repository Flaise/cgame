#include <stdbool.h>

typedef struct {
    int hover_x;
    int hover_y;
    int select_x;
    int select_y;
} Selection;

typedef struct {
    bool needs_redraw;
    Selection selection;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* tiles;
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
