#include <stdbool.h>

#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "constants.h"
#include "state.h"
#include "event.h"
#include "draw.h"
#include "terrain.h"

#include "res/floor.h"
/* #define RES_FLOOR __res_Floor___Grass_1_64x64_png */
#define RES_FLOOR __res_Floor___Dirt_2_64x64_png
#include "res/wall.h"
#define RES_WALL __res_Wall___Brick_1_64x64_png
/* #define RES_WALL __res_Wall___Brick_3_64x64_png */

int run_window(State* state) {
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
        "Let there be graphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        bounds.w, bounds.h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );
    if (win == NULL) {
        ERROR("SDL_CreateWindow");
    	return 1;
    }
    state->window = win;

    SDL_Renderer* renderer = SDL_CreateRenderer(
        win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    );
    if (renderer == NULL) {
        ERROR("SDL_CreateRenderer");
        return 1;
    }
    state->renderer = renderer;

    if (SDL_RenderSetLogicalSize(renderer, VIEW_WIDTH, VIEW_HEIGHT) != 0) {
        ERROR("SDL_RenderSetLogicalSize");
        return 1;
    }

    SDL_Texture* floor = const_png_to_texture(renderer, RES_FLOOR, sizeof(RES_FLOOR));
    if (floor == NULL) {
        ERROR("load PNG: floor");
        return 1;
    }
    state->floor = floor;
    
    SDL_Texture* wall = const_png_to_texture(renderer, RES_WALL, sizeof(RES_WALL));
    if (wall == NULL) {
        ERROR("load PNG: wall");
        return 1;
    }
    state->wall = wall;

    if (terrain_init(state) != 0) {
        return 1;
    }

    if (terrain_update(state) != 0) {
        WARN("terrain_update");
    }

    return all_events(state);
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

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        ERROR("IMG_Init: png");
        return 1;
    }
    
    return run_window(state);
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
