#include <stdio.h>
#include "SDL.h"

void log_message(char* level, char* file, int line, char* format, ...) {
    printf("%s [%s:%d] ", level, file, line);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    const char* err = SDL_GetError();
    if (err[0] != '\0') {
        /* append non-empty error string */
        
        printf(" -> %s", err);
        SDL_ClearError();
    }
    printf("\n");
}
