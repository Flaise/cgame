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

void redraw(State* state) {
    state->needs_redraw = true;
}

#define INSTRUCTIONS_WIDTH 351
#define INSTRUCTIONS_HEIGHT 94
void instructions_draw(State* state) {
    int32_t x = VIEW_WIDTH - INSTRUCTIONS_WIDTH / 2 - 5;
    int32_t y = 0;
    
    SDL_Rect dest_rect = {
        .x = x + 2,
        .y = y + 2,
        .w = INSTRUCTIONS_WIDTH / 2,
        .h = INSTRUCTIONS_HEIGHT / 2,
    };

    if (SDL_SetTextureColorMod(state->textures[TEXTURE_INSTRUCTIONS], 0, 0, 0) != 0) {
        WARN("SDL_SetTextureColorMod");
    }
    draw_texture(state, TEXTURE_INSTRUCTIONS, NULL, &dest_rect);

    dest_rect.x = x;
    dest_rect.y = y;
    
    if (SDL_SetTextureColorMod(state->textures[TEXTURE_INSTRUCTIONS], 255, 255, 255) != 0) {
        WARN("SDL_SetTextureColorMod");
    }
    draw_texture(state, TEXTURE_INSTRUCTIONS, NULL, &dest_rect);
}

void draw_loading_done() {
    if (IMG_Init(0) != 0) {
        IMG_Quit();
    }
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
        ERROR("load texture");
        return 1;
    }
    state->textures[texture_id] = texture;
    return 0;
}

void draw_texture(
        State* state, TexID texture_id, const SDL_Rect* source_rect, const SDL_Rect* dest_rect) {
        
    SDL_Texture* texture = state->textures[texture_id];
    if (texture == NULL) {
        WARN("Icon texture not loaded.");
        return;
    }
    if (SDL_RenderCopy(state->renderer, texture, source_rect, dest_rect) != 0) {
        WARN("SDL_RenderCopy");
        return;
    }
}
