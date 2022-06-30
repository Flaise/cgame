#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "entity.h"
#include "constants.h"

static bool in_view(int32_t x, int32_t y) {
    /* < VIEW_WIDTH because <= will still create a vertical bar when hovering */
    return x > 0 && y > 0 && x < VIEW_WIDTH && y < VIEW_HEIGHT;
}

void select_draw(State* state) {
    Selection* sel = &state->selection;

    if (sel->select_x >= 0 && sel->select_y >= 0) {
        if (SDL_SetRenderDrawColor(state->renderer, 110, 70, 60, 140) != 0) {
            WARN("SDL_SetRenderDrawColor");
        }

        SDL_Rect hover = {
            .x = sel->select_x * TILE_SIZE,
            .y = sel->select_y * TILE_SIZE,
            .w = TILE_SIZE,
            .h = TILE_SIZE,
        };

        if (SDL_RenderFillRect(state->renderer, &hover) != 0) {
            WARN("SDL_RenderFillRect");
        }
    }
    
    if (sel->hover_x >= 0 && sel->hover_y >= 0
    && !(sel->hover_x == sel->select_x && sel->hover_y == sel->select_y)) {
        if (SDL_SetRenderDrawColor(state->renderer, 40, 110, 90, 140) != 0) {
            WARN("SDL_SetRenderDrawColor");
        }

        SDL_Rect hover = {
            .x = sel->hover_x * TILE_SIZE,
            .y = sel->hover_y * TILE_SIZE,
            .w = TILE_SIZE,
            .h = TILE_SIZE,
        };

        if (SDL_RenderFillRect(state->renderer, &hover) != 0) {
            WARN("SDL_RenderFillRect");
        }
    }
}

void select_mouse_press(State* state, uint8_t button, int32_t x, int32_t y) {
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
}

void select_mouse_move(State* state, int32_t x, int32_t y) {
    Selection* sel = &state->selection;
    
    if (in_view(x, y)) {
        sel->hover_x = x / TILE_SIZE;
        sel->hover_y = y / TILE_SIZE;
    } else {
        sel->hover_x = -1;
        sel->hover_y = -1;
    }
}

void select_mouse_leave(State* state) {
    Selection* sel = &state->selection;
    sel->hover_x = -1;
    sel->hover_y = -1;
}
