#ifndef TEST

#include "SDL.h"
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
#include "cooldown.h"
#include "interact.h"
#include "tween.h"

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
#include "res/cooldown.h"
#define RES_COOLDOWN __res_hourglass2_png
#include "res/instructions.h"
#define RES_INSTRUCTIONS __res_instructions_png
#include "res/success.h"
#define RES_SUCCESS __res_success_png
#include "res/failure.h"
#define RES_FAILURE __res_failure_png

int window_icon_load_const_png(SDL_Window* window, const void* mem, int size) {
    SDL_Surface* surface = const_png_to_surface(mem, size);
    if (surface == NULL) {
        WARN("const_png_to_surface");
        return 1;
    }

    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);
    return 0;
}

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
        "Don't Eat My Sheep!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        bounds.w, bounds.h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );
    if (win == NULL) {
        ERROR("SDL_CreateWindow");
    	return 1;
    }
    state->window = win;

    if (window_icon_load_const_png(win, RES_KNIGHT, sizeof(RES_KNIGHT)) != 0) {
        WARN("window_icon_load_const_png");
    }

    SDL_EnableScreenSaver();

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
    if (texture_load_const_png(state, TEXTURE_COOLDOWN, RES_COOLDOWN, sizeof(RES_COOLDOWN)) != 0) {
        ERROR("texture_load_const_png (cooldown)");
        return 1;
    }
    if (texture_load_const_png(
            state, TEXTURE_INSTRUCTIONS, RES_INSTRUCTIONS, sizeof(RES_INSTRUCTIONS)) != 0) {
        ERROR("texture_load_const_png (instructions)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_SUCCESS, RES_SUCCESS, sizeof(RES_SUCCESS)) != 0) {
        ERROR("texture_load_const_png (cooldown)");
        return 1;
    }
    if (texture_load_const_png(state, TEXTURE_FAILURE, RES_FAILURE, sizeof(RES_FAILURE)) != 0) {
        ERROR("texture_load_const_png (cooldown)");
        return 1;
    }
    
    icon_texture_init(state, ICON_DRAGON, TEXTURE_DRAGON);
    icon_texture_init(state, ICON_KNIGHT, TEXTURE_KNIGHT);
    icon_texture_init(state, ICON_MKNIGHT, TEXTURE_MKNIGHT);
    icon_texture_init(state, ICON_SHEEP, TEXTURE_SHEEP);
    icon_texture_init(state, ICON_DOG, TEXTURE_DOG);
    icon_texture_init(state, ICON_HORSE, TEXTURE_HORSE);
    icon_texture_init(state, ICON_COOLDOWN, TEXTURE_COOLDOWN);
    icon_texture_init(state, ICON_INSTRUCTIONS, TEXTURE_INSTRUCTIONS);
    icon_texture_init(state, ICON_SUCCESS, TEXTURE_SUCCESS);
    icon_texture_init(state, ICON_FAILURE, TEXTURE_FAILURE);

    /* TODO: Should unload IMG in response to an event or something. */
    draw_loading_done();

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

    return events_all(state);
}

int main(int argc, char* argv[]) {
    State* state = state_new();
    if (state == NULL) {
        ERROR("make_state");
        return 1;
    }

    int status = run(state);

    /* clear state before SDL_Quit because it involves SDL calls */
    state_end(state);
    
    draw_loading_done();
    if (SDL_WasInit(0) != 0) {
        SDL_Quit();
    }
    
    return status;
}

#endif /* TEST */
