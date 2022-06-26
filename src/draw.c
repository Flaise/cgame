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
    if (SDL_SetRenderDrawColor(state->renderer, 60, 0, 0, 255) != 0) {
        WARN("SDL_SetRenderDrawColor");
    }

    if (SDL_RenderClear(state->renderer) != 0) {
        WARN("SDL_RenderClear");
    }
    
    SDL_Rect srcrect = {
        .x = 0,
        .y = 0,
        .w = 64,
        .h = 64,
    };

    if (SDL_RenderCopy(state->renderer, state->floor, &srcrect, &srcrect) != 0) {
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
    
    SDL_Surface* surf = IMG_LoadTyped_RW(rw, true, "PNG");
    /* IMG will close the RW even if there's an error while loading */
    return surf;
}

SDL_Texture* const_png_to_texture(SDL_Renderer* renderer, const void* mem, int size) {
    SDL_Surface* surf = const_png_to_surface(mem, size);
    if (surf == NULL) {
        return NULL;
    }

    SDL_Texture* result = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FreeSurface(surf);

    return result;
}
