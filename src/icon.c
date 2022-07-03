#include <stdbool.h>

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

Icon icon_new(TexID texture_id, SDL_Rect source_rect) {
    if (texture_id < 0 || texture_id >= TEXTURE_COUNT) {
        ERROR("invalid texture id");
        /* This function doesn't have a way to signal an error so just handle it in icon_draw(). */
    }
    
    Icon result = {
        .source_rect = source_rect,
        .texture_id = texture_id,
    };
    return result;
}

Icon icon_tile_new(TexID texture_id, int tile_size, int x, int y) {
    SDL_Rect source_rect = {
        .x = tile_size * x,
        .y = tile_size * y,
        .w = tile_size,
        .h = tile_size,
    };
    return icon_new(texture_id, source_rect);
}

void icon_tile_init(State* state, IconID icon_id, TexID texture_id, int tile_size, int x, int y) {
    state->icons[icon_id] = icon_tile_new(texture_id, tile_size, x, y);
}

void icon_texture_init(State* state, IconID icon_id, TexID texture_id) {
    uint32_t w, h;
    if (SDL_QueryTexture(state->textures[texture_id], NULL, NULL, &w, &h) != 0) {
        ERROR("SDL_QueryTexture");
        /* Not really necessary to signal an error because it's not recoverable. */
        return;
    }
    SDL_Rect source_rect = {
        .x = 0,
        .y = 0,
        .w = w,
        .h = h,
    };
    state->icons[icon_id] = icon_new(texture_id, source_rect);
}

void icon_draw(State* state, IconID icon_id, const SDL_Rect* dest_rect) {
    if (icon_id >= ICON_COUNT) {
        WARN("invalid icon ID");
        return;
    }
    
    const Icon* icon = &state->icons[icon_id];
    if (icon->texture_id < 0 || icon->texture_id >= TEXTURE_COUNT) {
        WARN("icon has invalid texture ID");
        return;
    }
    
    SDL_Texture* texture = state->textures[icon->texture_id];
    if (texture == NULL) {
        WARN("icon texture not found");
        return;
    }
    if (SDL_RenderCopy(state->renderer, texture, &icon->source_rect, dest_rect) != 0) {
        WARN("SDL_RenderCopy (icon)");
        return;
    }
}
