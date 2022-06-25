#include <stdbool.h>

#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"

#include "res/floor.h"
#define FLOOR __res_Floor___Grass_1_64x64_png

typedef enum {
    Proceed,
    Exit,
    Error,
} Status;

Status next_event() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return Exit;
        }
    }
    return Proceed;
}

int all_pending_events() {
    while (true) {
        Status stat = next_event();
        if (stat == Exit) {
            return 0;
        } else if (stat == Error) {
            return 1;
        }
    }
}

int run_window() {
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
        "Window be here", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        bounds.w, bounds.h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );
    if (win == NULL) {
        ERROR("SDL_CreateWindow");
    	return 1;
    }

    SDL_Surface* winsurf = SDL_GetWindowSurface(win);
    if (winsurf == NULL) {
        ERROR("SDL_GetWindowSurface");
        return 1;
    }

    SDL_RWops* floor_rw = SDL_RWFromConstMem(FLOOR, sizeof(FLOOR));
    SDL_Surface* floor;
    floor = IMG_LoadTyped_RW(floor_rw, true, "PNG");
    if (!floor) {
        ERROR("IMG_Load_RW: floor");
        return 1;
    }

    int status = all_pending_events();

    SDL_DestroyWindow(win);
    return status;
}

int run_img() {
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        ERROR("IMG_Init: png");
        return 1;
    }
    
    int status = run_window();
    
    IMG_Quit();
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
