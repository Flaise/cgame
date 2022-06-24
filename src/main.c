#ifdef _WIN32a
#include "SDL.h"
#else
#include <linux/time.h>
#define __timespec_defined 1
#define __timeval_defined 1
#define __itimerspec_defined 1
#include "SDL2/SDL.h"
#endif

void log_message(char* level, char* file, int line, char* message) {
    printf("%s [%s:%d] %s", level, file, line, message);
    
    const char* err = SDL_GetError();
    if (err[0] != '\0') {
        /* append non-empty error string */
        
        printf(" -> %s", err);
        SDL_ClearError();
    }
    printf("\n");
}

#define INFO(MESSAGE) log_message("[INFO] ", __FILE__, __LINE__, MESSAGE)
#define ERROR(MESSAGE) log_message("[ERROR]", __FILE__, __LINE__, MESSAGE)

int main(int argc, char* argv[]) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        ERROR("Unable to initialize SDL.");
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        ERROR("Unable to create window.");
    	SDL_Quit();
    	return 1;
    }

    ERROR("Test error message.");
    INFO("It works?");

    SDL_Quit();

    return 0;
}
