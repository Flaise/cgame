#include "SDL.h"
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
#include "tween.h"

void redraw(State* state) {
    state->needs_redraw = true;
}

#define INSTRUCTIONS_WIDTH 351
#define INSTRUCTIONS_HEIGHT 169
void instructions_draw(State* state) {
    int32_t x = VIEW_WIDTH - INSTRUCTIONS_WIDTH / 2 - 5;
    int32_t y = 0;
    
    SDL_Rect dest_rect = {
        .x = x + 2,
        .y = y + 2,
        .w = INSTRUCTIONS_WIDTH / 2,
        .h = INSTRUCTIONS_HEIGHT / 2,
    };

    icon_color_mod(state, ICON_INSTRUCTIONS, 0, 0, 0);
    icon_draw(state, ICON_INSTRUCTIONS, &dest_rect);

    dest_rect.x = x;
    dest_rect.y = y;
    
    icon_color_mod(state, ICON_INSTRUCTIONS, 255, 255, 255);
    icon_draw(state, ICON_INSTRUCTIONS, &dest_rect);
}

void draw_loading_done() {
    if (IMG_Init(0) != 0) {
        IMG_Quit();
    }
}

int draw_now(State* state) {
    tween_update(state);
    
    if (SDL_SetRenderTarget(state->renderer, NULL) != 0) {
        ERROR("SDL_SetRenderTarget");
        return 1;
    }

    /* Clear screen, including the space outside of the logical size/viewport. */
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
    instructions_draw(state);
    
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
    if (IMG_Init(0) == 0) {
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
            ERROR("IMG_Init");
            return NULL;
        }
    }
    
    SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
    if (rw == NULL) {
        return NULL;
    }
    
    return IMG_LoadTextureTyped_RW(renderer, rw, true, "PNG");
    /* IMG will close the RW even if there's an error while loading */
}

int texture_load_const_png(State* state, TexID texture_id, const void* mem, size_t size) {
    if (state->renderer == NULL) {
        ERROR("Renderer not initialized.");
        return 1;
    }
    if (state->textures[texture_id] != NULL) {
        ERROR("Texture already loaded.");
        return 1;
    }
    
    SDL_Texture* texture = const_png_to_texture(state->renderer, mem, size);
    if (texture == NULL) {
        ERROR("Could not load texture %d.", texture_id);
        return 1;
    }
    state->textures[texture_id] = texture;
    return 0;
}

SDL_Texture* draw_get_texture(State* state, TexID texture_id) {
    if (state == NULL) {
        ERROR("State can't be null.");
        return NULL;
    }
    
    if (texture_id < 0 || texture_id >= TEXTURE_COUNT) {
        WARN("Invalid texture ID %d.", texture_id);
        return NULL;
    }
    
    SDL_Texture* texture = state->textures[texture_id];
    if (texture == NULL) {
        WARN("Texture %d not loaded.", texture_id);
        return NULL;
    }
    
    return texture;
}

void draw_texture(
        State* state, TexID texture_id, const SDL_Rect* source_rect, const SDL_Rect* dest_rect) {
    SDL_Texture* texture = draw_get_texture(state, texture_id);
    if (texture == NULL) {
        return;
    }
    
    if (SDL_RenderCopy(state->renderer, texture, source_rect, dest_rect) != 0) {
        WARN("SDL_RenderCopy");
        return;
    }
}

void draw_texture_color_mod(State* state, TexID texture_id, uint8_t r, uint8_t g, uint8_t b) {
    SDL_Texture* texture = draw_get_texture(state, texture_id);
    if (texture == NULL) {
        return;
    }
    
    if (SDL_SetTextureColorMod(texture, r, g, b) != 0) {
        WARN("SDL_SetTextureColorMod");
        return;
    }
}

void draw_set_color(State* state, RGBA color) {
    if (state->renderer == NULL) {
        WARN("Renderer not initialized.");
        return;
    }
    
    int8_t r = color.r;
    int8_t g = color.g;
    int8_t b = color.b;
    int8_t a = color.a;
    if (SDL_SetRenderDrawColor(state->renderer, r, g, b, a) != 0) {
        WARN("SDL_SetRenderDrawColor");
    }
}
