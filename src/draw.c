#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "state.h"
#include "draw.h"

void redraw(State* state) {
    state->needs_redraw = true;
}

int draw_now(State* state) {
    if (SDL_SetRenderTarget(state->renderer, NULL) != 0) {
        ERROR("SDL_SetRenderTarget");
        return 1;
    }

    if (SDL_SetRenderDrawColor(state->renderer, 60, 0, 0, 255) != 0) {
        WARN("SDL_SetRenderDrawColor");
    }

    if (SDL_RenderClear(state->renderer) != 0) {
        WARN("SDL_RenderClear");
    }
    
    if (SDL_RenderCopy(state->renderer, state->terrain, NULL, NULL) != 0) {
        WARN("SDL_RenderCopy");
    }

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

SDL_Texture* const_png_to_texture(SDL_Renderer* renderer, const void* mem, int size) {
    SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
    if (rw == NULL) {
        return NULL;
    }
    
    return IMG_LoadTextureTyped_RW(renderer, rw, true, "PNG");
    /* IMG will close the RW even if there's an error while loading */
}
