#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "constants.h"
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

    SDL_Rect srcfloor = {
        .x = TILE_SIZE * 7,
        .y = TILE_SIZE * 5,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };
    SDL_Rect srcwall = {
        .x = TILE_SIZE * 6,
        .y = TILE_SIZE * 3,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };
    SDL_Rect dstrect = {
        .x = 0,
        .y = 0,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };
    
    for (int y = 1; y < TILES_DOWN - 1; y += 1) {
        for (int x = 1; x < TILES_ACROSS - 1; x += 1) {
            dstrect.x = x * TILE_SIZE;
            dstrect.y = y * TILE_SIZE;
            if (SDL_RenderCopy(state->renderer, state->floor, &srcfloor, &dstrect) != 0) {
                WARN("SDL_RenderCopy (floor)");
                break;
            }
        }
    }

    for (int y = 0; y < TILES_DOWN; y += 1) {
        for (int x = 0; x < TILES_ACROSS; x += 1) {
            if (!(x == 0 || y == 0 || x == TILES_ACROSS - 1 || y == TILES_DOWN - 1)) {
                continue;
            }
            dstrect.x = x * TILE_SIZE;
            dstrect.y = y * TILE_SIZE;
            if (SDL_RenderCopy(state->renderer, state->wall, &srcwall, &dstrect) != 0) {
                WARN("SDL_RenderCopy (wall)");
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

    SDL_Texture* terrain = SDL_CreateTexture(
        state->renderer, format, SDL_TEXTUREACCESS_TARGET, VIEW_WIDTH, VIEW_HEIGHT
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
