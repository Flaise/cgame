#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "constants.h"
#include "state.h"
#include "icon.h"

int terrain_update(State* state) {
    /* Start drawing to off-screen texture. */
    SDL_Texture* terrain_buf = state->textures[TEXTURE_TERRAIN];
    if (terrain_buf == NULL) {
        WARN("terrain buffer not initialized");
        return 1;
    }
    if (SDL_SetRenderTarget(state->renderer, terrain_buf) != 0) {
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

    /* Draw icons. */

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
            icon_draw(state, &state->icon_floor, &dstrect);
        }
    }

    for (int y = 0; y < TILES_DOWN; y += 1) {
        for (int x = 0; x < TILES_ACROSS; x += 1) {
            if (!(x == 0 || y == 0 || x == TILES_ACROSS - 1 || y == TILES_DOWN - 1)) {
                continue;
            }
            dstrect.x = x * TILE_SIZE;
            dstrect.y = y * TILE_SIZE;
            icon_draw(state, &state->icon_wall, &dstrect);
        }
    }
}

int terrain_init(State* state) {
    /* off screen texture to avoid seams between tiles */
    
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
    state->textures[TEXTURE_TERRAIN] = terrain;

    /* icons */

    state->icon_floor = icon_tile_init(TEXTURE_TILES, 32, 3, 5);
    state->icon_wall = icon_tile_init(TEXTURE_TILES, 32, 1, 2);
    
    return 0;
}

void terrain_draw(State* state) {
    if (SDL_RenderCopy(state->renderer, state->textures[TEXTURE_TERRAIN], NULL, NULL) != 0) {
        WARN("SDL_RenderCopy");
    }
}
