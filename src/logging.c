#include <stdio.h>

#ifdef _WIN32a
#include "SDL.h"
#else
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
