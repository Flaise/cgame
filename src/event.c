#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "state.h"
#include "draw.h"

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
                    /* int width = event.window.data1;
                       int height = event.window.data2; */

                    /* letterboxing is done automatically with SDL_RenderSetLogicalSize */
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
        } else {
            /* avoid hogging CPU when there's nothing to draw */
            SDL_Delay(10);
        }
    }
}
