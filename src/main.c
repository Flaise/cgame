#include <stdbool.h>

#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"

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

Status all_events() {
    while (true) {
        Status stat = next_event();
        if (stat != Proceed) {
            return stat;
        }
    }
}

int show_window() {
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
        ERROR("Unable to create window.");
    	return 1;
    }

    all_events();

    SDL_DestroyWindow(win);
    return 0;
}

/*
 apparently SDL_CreateWindow still works if SDL_Init is omitted
 SDL_Init doesn't error out if called twice
 SDL_CreateWindow still works after SDL_Quit
 */

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ERROR("Unable to initialize SDL.");
        return 1;
    }

    int status = show_window();
    
    SDL_Quit();
    return status;
}
