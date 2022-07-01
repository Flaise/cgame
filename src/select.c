#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "entity.h"
#include "constants.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA;

RGBA color_move_valid = {40, 110, 90, 120};
RGBA color_move_invalid = {150, 70, 60, 120};

RGBA color_selection = {150, 170, 60, 100};

RGBA color_select_valid = {40, 110, 90, 120};
RGBA color_select_empty = {40, 110, 90, 50};

static void set_color(State* state, RGBA color) {
    int8_t r = color.r;
    int8_t g = color.g;
    int8_t b = color.b;
    int8_t a = color.a;
    if (SDL_SetRenderDrawColor(state->renderer, r, g, b, a) != 0) {
        WARN("SDL_SetRenderDrawColor");
    }
}

static void draw_tile_rect(State* state, int32_t tile_x, int32_t tile_y) {
    SDL_Rect rect = {
        .x = tile_x * TILE_SIZE,
        .y = tile_y * TILE_SIZE,
        .w = TILE_SIZE,
        .h = TILE_SIZE,
    };

    if (SDL_RenderFillRect(state->renderer, &rect) != 0) {
        WARN("SDL_RenderFillRect");
    }
}

void select_draw(State* state) {
    Selection* sel = &state->selection;

    bool selection_visible = sel->select_x >= 0 && sel->select_y >= 0;
    bool hover_visible = sel->hover_x >= 0 && sel->hover_y >= 0;
    bool overlap = selection_visible && hover_visible
        && sel->select_x == sel->hover_x && sel->select_y == sel->hover_y;

    if (selection_visible) {
        RGBA color = color_selection;
        if (overlap) {
            color = color_move_invalid;
        }
        set_color(state, color);
        draw_tile_rect(state, sel->select_x, sel->select_y);
    }
    
    if (!overlap && hover_visible) {
        RGBA color;
        if (selection_visible) {
            color = color_move_valid;
        } else {
            color = color_select_valid;
        }
        set_color(state, color);
        draw_tile_rect(state, sel->hover_x, sel->hover_y);
    }
}

static bool in_view(int32_t x, int32_t y) {
    /* < VIEW_WIDTH because <= will still create a vertical bar when hovering */
    return x > 0 && y > 0 && x < VIEW_WIDTH && y < VIEW_HEIGHT;
}

static Entity selectable_at_lpixel(State* state, int32_t x, int32_t y) {
    if (!in_view(x, y)) {
        return 0;
    }
    
    CompGroup* groups[] = {
        &state->components.compgroups[COMPTYPE_SELECTABLE],
        &state->components.compgroups[COMPTYPE_POSITION],
    };
    void* comps[] = {NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 2)) {
        CPosition* position = (CPosition*)comps[1];

        if (x / TILE_SIZE == position->x && y / TILE_SIZE == position->y) {
            return position->entity;
        }
    }

    return 0;
}

void select_mouse_press(State* state, uint8_t button, int32_t x, int32_t y) {
    if (!(button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT)) {
        return;
    }
    
    Selection* sel = &state->selection;

    if (sel->select_x < 0 || sel->select_y < 0) {
        Entity subject = selectable_at_lpixel(state, x, y);
        if (subject != 0) {
            sel->select_x = x / TILE_SIZE;
            sel->select_y = y / TILE_SIZE;
            sel->subject = subject;
        }
    } else {
        if (sel->subject != 0) {
            CPosition* position = component_of(
                &state->components.compgroups[COMPTYPE_POSITION], sel->subject);
            if (position != NULL) {
                position->x = x / TILE_SIZE;
                position->y = y / TILE_SIZE;
            }
        }
        
        sel->select_x = -1;
        sel->select_y = -1;
        sel->subject = 0;
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
