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
    SDL_Window* win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        ERROR("Unable to create window.");
    	return 1;
    }

    ERROR("Test error message.");
    INFO("It works?");
    all_events();

    SDL_DestroyWindow(win);
    return 0;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        ERROR("Unable to initialize SDL.");
        return 1;
    }

    int status = show_window();
    
    SDL_Quit();
    return status;
}
