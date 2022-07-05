#include <stdio.h>
#include "SDL.h"

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
