#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "constants.h"
#include "state.h"
#include "draw.h"

typedef enum {
    Proceed,
    Exit,
    Error,
} Status;

void size_changed(State* state, int width, int height) {
    /* letterboxing is done automatically with SDL_RenderSetLogicalSize */
    redraw(state);
}

void mouse_motion(State* state, int x, int y) {
    /* < VIEW_WIDTH because <= will still create the vertical bar */
    if (x > 0 && y > 0 && x < VIEW_WIDTH && y < VIEW_HEIGHT) {
        state->hover_x = x / TILE_SIZE;
        state->hover_y = y / TILE_SIZE;
    } else {
        state->hover_x = -1;
        state->hover_y = -1;
    }

    redraw(state);
}

void mouse_leave(State* state) {
    state->hover_x = -1;
    state->hover_y = -1;
    redraw(state);
}

Status events_pending(State* state) {
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
                    size_changed(state, width, height);
                } else if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
                    mouse_leave(state);
                }
                continue;

            case SDL_MOUSEMOTION:
                /* SDL_GetMouseState doesn't report transformed positions so pass event data */
                mouse_motion(state, event.motion.x, event.motion.y);
                continue;
        }
    }
    return Proceed;
}

int events_all(State* state) {
    if (draw_now(state) != 0) {
        return 1;
    }
    
    while (true) {
        /* process event */
        Status stat = events_pending(state);
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

            /* TODO: framerate cap when vsync isn't available */
        } else {
            /* avoid hogging CPU when there's nothing to draw */
            SDL_Delay(10);
        }
    }
}
