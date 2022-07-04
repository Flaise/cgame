#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "state.h"
#include "icon.h"
#include "draw.h"
#include "terrain.h"
#include "select.h"
#include "avatar.h"
#include "cooldown.h"

void redraw(State* state) {
    state->needs_redraw = true;
}

int draw_now(State* state) {
    if (SDL_SetRenderTarget(state->renderer, NULL) != 0) {
        ERROR("SDL_SetRenderTarget");
        return 1;
    }

    /* clear screen (including the space outside of the logical size/viewport) */
    if (SDL_SetRenderDrawColor(state->renderer, 30, 0, 0, 255) != 0) {
        WARN("SDL_SetRenderDrawColor");
    }
    if (SDL_RenderClear(state->renderer) != 0) {
        WARN("SDL_RenderClear");
    }

    terrain_draw(state);
    select_draw(state);
    avatar_draw(state);
    cooldown_draw(state);
    
    SDL_RenderPresent(state->renderer);

    state->needs_redraw = false;
    return 0;
}

SDL_Surface* const_png_to_surface(const void* mem, int size) {
    SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
    if (rw == NULL) {
        return NULL;
    }
    
    return IMG_LoadTyped_RW(rw, true, "PNG");
    /* IMG will close the RW even if there's an error while loading */
}

SDL_Texture* const_png_to_texture(SDL_Renderer* renderer, const void* mem, size_t size) {
    SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
    if (rw == NULL) {
        return NULL;
    }
    
    return IMG_LoadTextureTyped_RW(renderer, rw, true, "PNG");
    /* IMG will close the RW even if there's an error while loading */
}

int texture_load_const_png(State* state, TexID texture_id, const void* mem, size_t size) {
    if (state->renderer == NULL) {
        ERROR("renderer not initialized");
        return 1;
    }
    if (state->textures[texture_id] != NULL) {
        ERROR("texture already loaded");
        return 1;
    }
    
    SDL_Texture* texture = const_png_to_texture(state->renderer, mem, size);
    if (texture == NULL) {
        ERROR("load texture");
        return 1;
    }
    state->textures[texture_id] = texture;
    return 0;
}
