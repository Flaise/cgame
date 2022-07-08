
Icon icon_new(TexID texture_id, SDL_Rect source_rect);

Icon icon_tile_new(TexID texture_id, int32_t tile_size, int32_t x, int32_t y);

void icon_tile_init(
    State* state, IconID icon_id, TexID texture_id, int32_t tile_size, int32_t x, int32_t y);

void icon_texture_init(State* state, IconID icon_id, TexID texture_id);

void icon_draw(State* state, IconID icon_id, const SDL_Rect* dest_rect);

void icon_color_mod(State* state, IconID icon_id, uint8_t r, uint8_t g, uint8_t b);
