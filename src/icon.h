
Icon icon_init(TexID texture_id, SDL_Rect source_rect);
Icon icon_tile_init(TexID texture_id, int tile_size, int x, int y);
void icon_draw(State* state, const Icon* icon, const SDL_Rect* dest_rect);
