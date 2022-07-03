
Icon icon_new(TexID texture_id, SDL_Rect source_rect);

Icon icon_tile_new(TexID texture_id, int tile_size, int x, int y);

void icon_tile_init(State* state, IconID icon_id, TexID texture_id, int tile_size, int x, int y);

void icon_texture_init(State* state, IconID icon_id, TexID texture_id);

void icon_draw(State* state, IconID icon_id, const SDL_Rect* dest_rect);
