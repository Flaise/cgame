#ifndef TEST

#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "state.h"
#include "icon.h"
#include "event.h"
#include "draw.h"
#include "terrain.h"
#include "select.h"
#include "board.h"

#include "res/terrain.h"
#define RES_TILES __res_Tiny_Top_Down_32x32_png
#include "res/dragon.h"
#define RES_DRAGON __res_dragon_png
#include "res/knight.h"
#define RES_KNIGHT __res_knight_png
#include "res/knight_mounted.h"
#define RES_MKNIGHT __res_knight_mounted_png
#include "res/sheep.h"
#define RES_SHEEP __res_sheep_png
#include "res/dog.h"
#define RES_DOG __res_dog_png
#include "res/horse.h"
#define RES_HORSE __res_horse_png

int window_init(State* state) {
    SDL_Rect bounds;
    if (SDL_GetDisplayUsableBounds(0, &bounds) != 0) {
        WARN("SDL_GetDisplayUsableBounds");

        bounds.w = 640;
        bounds.h = 480;
    } else {
        /* demaximize to a window area that leaves gaps */
        bounds.w = bounds.w * 3 / 4;
        bounds.h = bounds.h * 3 / 4;
    }

    /* On Ubuntu, setting a window close to the usable bounds seems to implicitly make it act
       like a maximized window. */

    SDL_Window* win = SDL_CreateWindow(
        "Almost a puzzle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        bounds.w, bounds.h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );
    if (win == NULL) {
        ERROR("SDL_CreateWindow");
    	return 1;
    }
    state->window = win;

    return 0;
}

int renderer_init(State* state) {
    SDL_Renderer* renderer = SDL_CreateRenderer(
        state->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    );
    if (renderer == NULL) {
        ERROR("SDL_CreateRenderer");
        return 1;
    }
    state->renderer = renderer;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (SDL_RenderSetLogicalSize(renderer, VIEW_WIDTH, VIEW_HEIGHT) != 0) {
        ERROR("SDL_RenderSetLogicalSize");
        return 1;
    }

    return 0;
}

int textures_init(State* state) {
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        ERROR("IMG_Init: png");
        return 1;
    }
    
    if (texture_load_const_png(state, TEXTURE_TILES, RES_TILES, sizeof(RES_TILES)) != 0) {
        ERROR("texture_load_const_png (tiles)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_DRAGON, RES_DRAGON, sizeof(RES_DRAGON)) != 0) {
        ERROR("texture_load_const_png (dragon)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_KNIGHT, RES_KNIGHT, sizeof(RES_KNIGHT)) != 0) {
        ERROR("texture_load_const_png (knight)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_MKNIGHT, RES_MKNIGHT, sizeof(RES_MKNIGHT)) != 0) {
        ERROR("texture_load_const_png (mounted knight)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_SHEEP, RES_SHEEP, sizeof(RES_SHEEP)) != 0) {
        ERROR("texture_load_const_png (sheep)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_DOG, RES_DOG, sizeof(RES_DOG)) != 0) {
        ERROR("texture_load_const_png (dog)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_HORSE, RES_HORSE, sizeof(RES_HORSE)) != 0) {
        ERROR("texture_load_const_png (horse)");
        return 1;
    }

    /* TODO: icon_texture_init for entire texture */
    state->icon_dragon = icon_tile_init(TEXTURE_DRAGON, 128, 0, 0);
    state->icon_knight = icon_tile_init(TEXTURE_KNIGHT, 128, 0, 0);
    state->icon_mknight = icon_tile_init(TEXTURE_MKNIGHT, 128, 0, 0);
    state->icon_sheep = icon_tile_init(TEXTURE_SHEEP, 128, 0, 0);
    state->icon_dog = icon_tile_init(TEXTURE_DOG, 128, 0, 0);
    state->icon_horse = icon_tile_init(TEXTURE_HORSE, 128, 0, 0);

    /* Unconditionally unload IMG because no more textures will be loaded. */
    IMG_Quit();
    return 0;
}

/*
 apparently SDL_CreateWindow still works if SDL_Init is omitted
 SDL_Init doesn't error out if called twice
 SDL_CreateWindow still works after SDL_Quit
 */

int run(State* state) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ERROR("SDL_Init");
        return 1;
    }
    
    if (window_init(state) != 0) {
        return 1;
    }
    if (renderer_init(state) != 0) {
        return 1;
    }
    if (textures_init(state) != 0) {
        return 1;
    }
    if (terrain_init(state) != 0) {
        return 1;
    }
    if (terrain_update(state) != 0) {
        WARN("terrain_update");
    }

    level_1_init(state);

    return events_all(state);
}

int main(int argc, char* argv[]) {
    State* state = make_state();
    if (state == NULL) {
        ERROR("make_state");
        return 1;
    }

    int status = run(state);

    /* clear state before SDL_Quit because it involves SDL calls */
    destroy_state(state);
    
    if (IMG_Init(0) != 0) {
        IMG_Quit();
    }
    if (SDL_WasInit(0) != 0) {
        SDL_Quit();
    }
    
    return status;
}

#endif /* TEST */
