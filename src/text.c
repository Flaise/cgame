#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "draw.h"

#define INSTRUCTIONS_WIDTH 351
#define INSTRUCTIONS_HEIGHT 169

static void instructions_draw(State* state) {
    SDL_Rect dest_rect = {
        .x = VIEW_WIDTH - INSTRUCTIONS_WIDTH / 2 - 5,
        .y = 0,
        .w = INSTRUCTIONS_WIDTH / 2,
        .h = INSTRUCTIONS_HEIGHT / 2,
    };
    draw_text_shadow(state, ICON_INSTRUCTIONS, dest_rect);
}

#define FAILURE_WIDTH 186
#define FAILURE_HEIGHT 51

#define SUCCESS_WIDTH 232
#define SUCCESS_HEIGHT 48

void text_draw(State* state) {
    instructions_draw(state);

    if (state->game_over) {
        if (state->won) {
            SDL_Rect dest_rect = {
                .x = VIEW_WIDTH / 2 - SUCCESS_WIDTH / 4,
                .y = VIEW_HEIGHT / 2 - SUCCESS_HEIGHT / 4,
                .w = SUCCESS_WIDTH / 2,
                .h = SUCCESS_HEIGHT / 2,
            };
            draw_text_shadow(state, ICON_SUCCESS, dest_rect);
        } else {
            SDL_Rect dest_rect = {
                .x = VIEW_WIDTH / 2 - FAILURE_WIDTH / 4,
                .y = VIEW_HEIGHT / 2 - FAILURE_HEIGHT / 4,
                .w = FAILURE_WIDTH / 2,
                .h = FAILURE_HEIGHT / 2,
            };
            draw_text_shadow(state, ICON_FAILURE, dest_rect);
        }
    }
}
