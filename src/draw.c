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
    SDL_Surface* screen = SDL_GetWindowSurface(state->window);
    if (screen == NULL) {
        ERROR("SDL_GetWindowSurface");
        return 1;
    }
    
    if (SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 60, 0, 0)) != 0) {
        WARN("SDL_FillRect");
    }
    
    SDL_Rect srcrect = {
        .x = 0,
        .y = 0,
        .w = 64,
        .h = 64,
    };

    if (SDL_BlitSurface(state->floor, &srcrect, screen, NULL) != 0) {
        WARN("SDL_BlitSurface");
    }

    if (SDL_UpdateWindowSurface(state->window) != 0) {
        WARN("SDL_UpdateWindowSurface");
    }

    state->needs_redraw = false;
    return 0;
}

SDL_Surface* const_png_to_surface(const void *mem, int size) {
    SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
    if (!rw) {
        return NULL;
    }
    
    SDL_Surface* surf = IMG_LoadTyped_RW(rw, true, "PNG");
    if (!surf) {
        /* IMG will close the RW even if there's an error while loading */
        return NULL;
    }

    return surf;
}
