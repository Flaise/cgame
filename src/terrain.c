#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
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

    /* Default floor tiles. */

    SDL_Rect dest_rect = {
        .x = 0,
        .y = 0,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };
    for (int y = 0; y < TILES_DOWN; y += 1) {
        for (int x = 0; x < TILES_ACROSS; x += 1) {
            dest_rect.x = x * TILE_SIZE;
            dest_rect.y = y * TILE_SIZE;

            IconID icon = ICON_FLOOR_A;
            if ((x + y) % 2 == 0) {
                icon = ICON_FLOOR_B;
            }
            
            icon_draw(state, icon, &dest_rect);
        }
    }

    /* Dynamic terrain tiles. */

    CompGroup* groups[] = {
        &state->components.compgroups[COMPTYPE_TILE],
        &state->components.compgroups[COMPTYPE_POSITION],
    };
    void* comps[] = {NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 2)) {
        CTile* tile = (CTile*)comps[0];
        CPosition* position = (CPosition*)comps[1];

        dest_rect.x = TILE_SIZE * position->x;
        dest_rect.y = TILE_SIZE * position->y;

        icon_draw(state, tile->icon_id, &dest_rect);
    }
}

int terrain_init(State* state) {
    /* Off screen texture to avoid seams between tiles. */
    
    uint32_t format = SDL_GetWindowPixelFormat(state->window);
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

    /* Initialize tiles. */

    icon_tile_init(state, ICON_FLOOR_A, TEXTURE_TILES, 32, 3, 7);
    icon_tile_init(state, ICON_FLOOR_B, TEXTURE_TILES, 32, 1, 7);
    icon_tile_init(state, ICON_WALL, TEXTURE_TILES, 32, 1, 2);
    icon_tile_init(state, ICON_PYRAMID, TEXTURE_TILES, 32, 1, 0);
    
    return 0;
}

void terrain_draw(State* state) {
    if (SDL_RenderCopy(state->renderer, state->textures[TEXTURE_TERRAIN], NULL, NULL) != 0) {
        WARN("SDL_RenderCopy");
    }
}
