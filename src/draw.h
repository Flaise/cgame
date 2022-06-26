
void redraw(State* state);

int draw_now(State* state);

/*
    Returns: A newly allocated SDL_Surface, owned by the caller, or NULL if there was an error.
    Check the SDL error with the ERROR() macro.
*/
SDL_Surface* const_png_to_surface(const void* mem, int size);

/*
    Returns: A newly allocated SDL_Texture, owned by the caller, or NULL if there was an error.
    Check the SDL error with the ERROR() macro.
*/
SDL_Texture* const_png_to_texture(SDL_Renderer* renderer, const void* mem, int size);
