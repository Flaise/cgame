
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
SDL_Texture* const_png_to_texture(SDL_Renderer* renderer, const void* mem, size_t size);

/*
    Loads a texture from const memory and adds it to the state for later reference.
    Returns: 0 if successful
 */
int texture_load_const_png(State* state, TexID texture_id, const void* mem, size_t size);
