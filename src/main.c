// #include <SDL2/SDL.h>

#ifdef _WIN32a
#include "SDL.h"
#else
#include <linux/time.h>
#define __timespec_defined 1
#define __timeval_defined 1
#define __itimerspec_defined 1
#include "SDL2/SDL.h"
#endif


int main(int argc, char* argv[]) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to create window: %s", SDL_GetError());
    	SDL_Quit();
    	return 1;
    }

    SDL_Log("It works?");

    SDL_Quit();

    return 0;
}
