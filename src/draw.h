
void redraw(State* state);

int draw_now(State* state);

/*
    Returns: A newly allocated SDL_Surface, owned by the caller, or NULL if there was an error.
    Check the SDL error with the ERROR() macro.
*/
SDL_Surface* const_png_to_surface(const void *mem, int size);
