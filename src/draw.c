#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "constants.h"
#include "state.h"
#include "draw.h"
#include "terrain.h"

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

    if (state->hover_x >= 0 && state->hover_y >= 0) {
        if (SDL_SetRenderDrawColor(state->renderer, 40, 110, 90, 140) != 0) {
            WARN("SDL_SetRenderDrawColor");
        }

        SDL_Rect hover = {
            .x = state->hover_x * TILE_SIZE,
            .y = state->hover_y * TILE_SIZE,
            .w = TILE_SIZE,
            .h = TILE_SIZE,
        };

        if (SDL_RenderFillRect(state->renderer, &hover) != 0) {
            WARN("SDL_RenderFillRect");
        }
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
