#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "state.h"

int terrain_update(State* state) {
    if (state->terrain == NULL) {
        WARN("state->terrain not set");
        return 1;
    }
    
    if (SDL_SetRenderTarget(state->renderer, state->terrain) != 0) {
        ERROR("SDL_SetRenderTarget");
        return 1;
    }

    /* Turn everything magenta to show undrawn locations. */
    if (SDL_SetRenderDrawColor(state->renderer, 255, 0, 255, 255) != 0) {
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
    SDL_Rect dstrect = {
        .x = 0,
        .y = 0,
        .w = 64,
        .h = 64,
    };

    for (int y = 0; y < 8; y += 1) {
        for (int x = 0; x < 8; x += 1) {
            dstrect.x = x * 64;
            dstrect.y = y * 64;
            if (SDL_RenderCopy(state->renderer, state->floor, &srcrect, &dstrect) != 0) {
                WARN("SDL_RenderCopy");
                break;
            }
        }
    }
}

int terrain_init(State* state) {
    Uint32 format = SDL_GetWindowPixelFormat(state->window);
    if (format == SDL_PIXELFORMAT_UNKNOWN) {
        WARN("SDL_GetWindowPixelFormat");
        
        format = SDL_PIXELFORMAT_RGBA8888;
    }

    int tiles_across = 8;
    int tiles_down = 8;
    int terrain_w = tiles_across * 64;
    int terrain_h = tiles_down * 64;
    SDL_Texture* terrain = SDL_CreateTexture(
        state->renderer, format, SDL_TEXTUREACCESS_TARGET, terrain_w, terrain_h
    );
    if (terrain == NULL) {
        ERROR("SDL_CreateTexture (terrain)");
        return 1;
    }
    state->terrain = terrain;
    
    return 0;
}

void terrain_draw(State* state) {
    if (SDL_RenderCopy(state->renderer, state->terrain, NULL, NULL) != 0) {
        WARN("SDL_RenderCopy");
    }
}
