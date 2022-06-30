#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "entity.h"
#include "constants.h"

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
