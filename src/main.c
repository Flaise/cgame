#include <stdbool.h>

#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "math.h"
#include "state.h"
#include "draw.h"

#include "res/floor.h"
#define FLOOR __res_Floor___Grass_1_64x64_png

typedef enum {
    Proceed,
    Exit,
    Error,
} Status;

Status next_event(State* state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return Exit;

            /* on Ubuntu, drag-resize = SizeChanged
               double click to unmaximize or click unmaximize button = SizeChanged->Restored
               click on task bar to unminimize = Restored */
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width = event.window.data1;
                    int height = event.window.data2;

                    /* TODO: do something with w/h? */

                    redraw(state);
                }
                continue;
        }
    }
    return Proceed;
}

int all_events(State* state) {
    if (draw_now(state) != 0) {
        return 1;
    }
    
    while (true) {
        /* process event */
        Status stat = next_event(state);
        if (stat == Exit) {
            return 0;
        } else if (stat == Error) {
            return 1;
        }

        /* redraw */
        if (state->needs_redraw) {
            if (draw_now(state) != 0) {
                return 1;
            }
        }
    }
}

int update_terrain(State* state) {
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
    
    if (SDL_RenderCopy(state->renderer, state->floor, NULL, NULL) != 0) {
        WARN("SDL_RenderCopy");
    }
}

int make_terrain(State* state) {
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
        "Some day it'll be a game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

    SDL_Texture* floor = const_png_to_texture(renderer, FLOOR, sizeof(FLOOR));
    if (floor == NULL) {
        ERROR("load PNG: floor");
        return 1;
    }
    state->floor = floor;

    if (make_terrain(state) != 0) {
        return 1;
    }

    if (update_terrain(state) != 0) {
        WARN("update_terrain failed");
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
