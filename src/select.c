#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "draw.h"
#include "interact.h"

RGBA color_move_valid = {40, 130, 100, 130};
RGBA color_move_invalid = {150, 70, 60, 150};

RGBA color_selection = {150, 170, 60, 130};

RGBA color_select_valid = {40, 130, 100, 130};
RGBA color_select_empty = {40, 45, 40, 90};
RGBA color_select_invalid = {150, 70, 60, 150};

static void tile_rect_draw(State* state, Coord tile_x, Coord tile_y) {
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
        draw_set_color(state, color);
        tile_rect_draw(state, sel->select_x, sel->select_y);
    }
    
    if (!overlap && hover_visible) {
        RGBA color;
        if (selection_visible) {
            if (sel->hover_status == HoverInvalid) {
                color = color_move_invalid;
            } else {
                color = color_move_valid;
            }
        } else if (sel->hover_status == HoverEmpty) {
            color = color_select_empty;
        } else if (sel->hover_status == HoverValid) {
            color = color_select_valid;
        } else if (sel->hover_status == HoverInvalid) {
            color = color_select_invalid;
        } else {
#ifdef DEBUG
            ERROR("Unreachable.");
#endif
            color = color_select_invalid;
        }
        draw_set_color(state, color);
        tile_rect_draw(state, sel->hover_x, sel->hover_y);
    }
}

static bool in_view(int32_t x, int32_t y) {
    /* < VIEW_WIDTH because <= will still create a vertical bar when hovering */
    return x >= 0 && y >= 0 && x < VIEW_WIDTH && y < VIEW_HEIGHT;
}

static void update_validity(State* state, int32_t x, int32_t y) {
    if (!in_view(x, y)) {
        return;
    }
    Coord tdest_x = x / TILE_SIZE;
    Coord tdest_y = y / TILE_SIZE;
    Selection* sel = &state->selection;
    
    if (sel->subject == 0) {
        /* No piece selected. */
        Entity target = type_at(state, COMPTYPE_SELECTABLE, tdest_x, tdest_y);
        if (target == 0) {
            /* No selectable piece. */
            Entity av = type_at(state, COMPTYPE_AVATAR, tdest_x, tdest_y);
            if (av == 0) {
                /* No mobile/avatar piece; only terrain. */
                sel->hover_status = HoverEmpty;
            } else {
                /* Unselectable piece. */
                sel->hover_status = HoverInvalid;
            }
        } else {
            bool is_cd =
                (component_of(&state->components.compgroups[COMPTYPE_COOLDOWN], target) != NULL);
            if (is_cd) {
                sel->hover_status = HoverInvalid;
            } else {
                sel->hover_status = HoverValid;
            }
        }
    } else {
        /* Piece selected. */
        if (will_move(state, sel->subject, tdest_x, tdest_y)) {
            sel->hover_status = HoverValid;
        } else {
            sel->hover_status = HoverInvalid;
        }
    }
}

void select_mouse_press(State* state, uint8_t button, int32_t x, int32_t y) {
    if (!(button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT)) {
        return;
    }
    if (!in_view(x, y)) {
        return;
    }
    int32_t tile_x = x / TILE_SIZE;
    int32_t tile_y = y / TILE_SIZE;
    Selection* sel = &state->selection;

    if (sel->select_x < 0 || sel->select_y < 0) {
        Entity subject = type_at(state, COMPTYPE_SELECTABLE, tile_x, tile_y);
        if (subject != 0) {
            bool is_cd =
                (component_of(&state->components.compgroups[COMPTYPE_COOLDOWN], subject) != NULL);
            if (!is_cd) {
                sel->select_x = tile_x;
                sel->select_y = tile_y;
                sel->subject = subject;
            }
        }
    } else {
        if (sel->subject != 0) {
            command_move(state, sel->subject, tile_x, tile_y);
        }
        
        sel->select_x = -1;
        sel->select_y = -1;
        sel->subject = 0;
    }
    update_validity(state, x, y);
}

void select_mouse_move(State* state, int32_t x, int32_t y) {
    Selection* sel = &state->selection;
    
    if (in_view(x, y)) {
        sel->hover_x = x / TILE_SIZE;
        sel->hover_y = y / TILE_SIZE;

        update_validity(state, x, y);
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
