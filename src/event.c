#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
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

bool in_view(Sint32 x, Sint32 y) {
    /* < VIEW_WIDTH because <= will still create a vertical bar when hovering */
    return x > 0 && y > 0 && x < VIEW_WIDTH && y < VIEW_HEIGHT;
}

void mouse_motion(State* state, Sint32 x, Sint32 y) {
    Selection* sel = &state->selection;
    
    if (in_view(x, y)) {
        sel->hover_x = x / TILE_SIZE;
        sel->hover_y = y / TILE_SIZE;
    } else {
        sel->hover_x = -1;
        sel->hover_y = -1;
    }

    redraw(state);
}

void mouse_leave(State* state) {
    Selection* sel = &state->selection;
    sel->hover_x = -1;
    sel->hover_y = -1;
    
    redraw(state);
}

void mouse_button(State* state, Uint8 button, Sint32 x, Sint32 y) {
    if (!(button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT)) {
        return;
    }
    
    Selection* sel = &state->selection;

    if (in_view(x, y) && (sel->select_x < 0 || sel->select_y < 0)) {
        sel->select_x = x / TILE_SIZE;
        sel->select_y = y / TILE_SIZE;
    } else {
        sel->select_x = -1;
        sel->select_y = -1;
    }
    
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

            case SDL_MOUSEBUTTONDOWN:
                mouse_button(state, event.button.button, event.button.x, event.button.y);
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
