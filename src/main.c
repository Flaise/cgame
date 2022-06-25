#include <stdbool.h>

#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "state.h"

#include "res/floor.h"
#define FLOOR __res_Floor___Grass_1_64x64_png

typedef enum {
    Proceed,
    Exit,
    Error,
} Status;

void redraw(State* state) {
    state->needs_redraw = true;
}

int draw_now(State* state) {
    SDL_Surface* screen = SDL_GetWindowSurface(state->window);
    if (screen == NULL) {
        ERROR("SDL_GetWindowSurface");
        return 1;
    }
    
    if (SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 60, 0, 0)) != 0) {
        WARN("SDL_FillRect");
    }
    
    SDL_Rect srcrect = {
        .x = 0,
        .y = 0,
        .w = 64,
        .h = 64,
    };

    if (SDL_BlitSurface(state->floor, &srcrect, screen, NULL) != 0) {
        WARN("SDL_BlitSurface");
    }

    if (SDL_UpdateWindowSurface(state->window) != 0) {
        WARN("SDL_UpdateWindowSurface");
    }

    state->needs_redraw = false;
    return 0;
}

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

int all_pending_events(State* state) {
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

/*
    Returns: A newly allocated SDL_Surface, owned by the caller, or NULL if there was an error.
    Check the SDL error with the ERROR() macro.
*/
SDL_Surface* const_png_to_surface(const void *mem, int size) {
    SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
    if (!rw) {
        return NULL;
    }
    
    SDL_Surface* surf = IMG_LoadTyped_RW(rw, true, "PNG");
    if (!surf) {
        /* IMG will close the RW even if there's an error while loading */
        return NULL;
    }

    return surf;
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

    SDL_Surface* floor = const_png_to_surface(FLOOR, sizeof(FLOOR));
    if (floor == NULL) {
        ERROR("load PNG: floor");
        return 1;
    }
    state->floor = floor;

    if (draw_now(state) != 0) {
        return 1;
    }

    return all_pending_events(state);
}

int run_img() {
    State* state = make_state();
    if (state == NULL) {
        ERROR("make_state");
        return 1;
    }
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        ERROR("IMG_Init: png");
        return 1;
    }
    
    int status = run_window(state);
    
    IMG_Quit();
    destroy_state(state);
    return status;
}

/*
 apparently SDL_CreateWindow still works if SDL_Init is omitted
 SDL_Init doesn't error out if called twice
 SDL_CreateWindow still works after SDL_Quit
 */

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ERROR("SDL_Init");
        return 1;
    }

    int status = run_img();
    
    SDL_Quit();
    return status;
}
