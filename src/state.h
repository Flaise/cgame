
typedef struct {
    SDL_Window* window;
    SDL_Surface* screen;
} State;

/*
    Allocates and returns a new blank State. Caller must free the state. Returns NULL if the
    allocation failed.
*/
State* make_state();
