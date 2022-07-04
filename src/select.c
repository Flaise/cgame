#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA;

RGBA color_move_valid = {40, 130, 100, 130};
RGBA color_move_invalid = {150, 70, 60, 150};

RGBA color_selection = {150, 170, 60, 130};

RGBA color_select_valid = {40, 130, 100, 130};
RGBA color_select_empty = {40, 130, 100, 70};

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
            if (sel->hover_valid) {
                color = color_move_valid;
            } else {
                color = color_move_invalid;
            }
        } else {
            if (sel->hover_valid) {
                color = color_select_valid;
            } else {
                color = color_select_empty;
            }
        }
        set_color(state, color);
        draw_tile_rect(state, sel->hover_x, sel->hover_y);
    }
}

static bool in_view(int32_t x, int32_t y) {
    /* < VIEW_WIDTH because <= will still create a vertical bar when hovering */
    return x >= 0 && y >= 0 && x < VIEW_WIDTH && y < VIEW_HEIGHT;
}

static bool in_board(int32_t tile_x, int32_t tile_y) {
    return tile_x >= 0 && tile_y >= 0 && tile_x < TILES_ACROSS && tile_y < TILES_DOWN;
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

static Entity type_at(State* state, uint8_t comptype, int32_t tile_x, int32_t tile_y) {
    CompGroup* groups[] = {
        &state->components.compgroups[comptype],
        &state->components.compgroups[COMPTYPE_POSITION],
    };
    void* comps[] = {NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 2)) {
        CPosition* position = (CPosition*)comps[1];

        if (tile_x == position->x && tile_y == position->y) {
            return position->entity;
        }
    }

    return 0;
}

static int32_t distance4(int32_t ax, int32_t ay, int32_t bx, int32_t by) {
    return abs(ax - bx) + abs(ay - by);
}

static bool will_interact(State* state, Entity subject, int32_t tile_x, int32_t tile_y) {
    Entity mount = type_at(state, COMPTYPE_MOUNT, tile_x, tile_y);
    bool is_rider = 
        (component_of(&state->components.compgroups[COMPTYPE_RIDER], subject) != NULL);
    if (mount != 0 && is_rider) {
        return true;
    }
    
    Entity edible = type_at(state, COMPTYPE_EDIBLE, tile_x, tile_y);
    bool is_munch =
        (component_of(&state->components.compgroups[COMPTYPE_MUNCH], subject) != NULL);
    if (edible != 0 && is_munch) {
        return true;
    }
    
    Entity slayme = type_at(state, COMPTYPE_SLAYME, tile_x, tile_y);
    bool is_slayer =
        (component_of(&state->components.compgroups[COMPTYPE_SLAYER], subject) != NULL);
    if (slayme != 0 && is_slayer) {
        return true;
    }

    return false;
}

static void update_validity(State* state, int32_t x, int32_t y) {
    Selection* sel = &state->selection;
    
    bool selection_visible = sel->select_x >= 0 && sel->select_y >= 0;

    int32_t tdest_x = x / TILE_SIZE;
    int32_t tdest_y = y / TILE_SIZE;
    if (selection_visible) {
        if (distance4(tdest_x, tdest_y, sel->select_x, sel->select_y) != 1) {
            sel->hover_valid = false;
        } else {
            /* TODO: Just store the subject in the selection struct. */
            Entity subject = selectable_at_lpixel(
                state, sel->select_x * TILE_SIZE, sel->select_y * TILE_SIZE);
            
            if (will_interact(state, subject, tdest_x, tdest_y)) {
                sel->hover_valid = true;
            } else {
                Entity target = type_at(state, COMPTYPE_OBSTRUCTION, tdest_x, tdest_y);
                sel->hover_valid = (target == 0);
            }
        }
    } else {
        Entity subject = selectable_at_lpixel(state, x, y);
        sel->hover_valid = (subject != 0);
    }
}

static bool interact(State* state, Entity subject, int32_t tile_x, int32_t tile_y) {
    bool interacted = false;

    /* knight + horse = mounted */
    Entity mount = type_at(state, COMPTYPE_MOUNT, tile_x, tile_y);
    bool is_rider = 
        (component_of(&state->components.compgroups[COMPTYPE_RIDER], subject) != NULL);
    if (mount != 0 && is_rider) {
        components_entity_end(&state->components, mount);
        component_end(&state->components.compgroups[COMPTYPE_RIDER], subject);

        CAvatar* avatar =
            (CAvatar*)component_of(&state->components.compgroups[COMPTYPE_AVATAR], subject);
        if (avatar != NULL) {
            avatar->icon_id = ICON_MKNIGHT;
        }
        slayer_init(&state->components, subject);
        
        interacted = true;
    }

    /* draggy + livestock = munch */
    Entity edible = type_at(state, COMPTYPE_EDIBLE, tile_x, tile_y);
    bool is_munch =
        (component_of(&state->components.compgroups[COMPTYPE_MUNCH], subject) != NULL);
    if (edible != 0 && is_munch) {
        components_entity_end(&state->components, edible);
        
        interacted = true;
    }

    /* knight + draggy = yay */
    Entity slayme = type_at(state, COMPTYPE_SLAYME, tile_x, tile_y);
    bool is_slayer =
        (component_of(&state->components.compgroups[COMPTYPE_SLAYER], subject) != NULL);
    if (slayme != 0 && is_slayer) {
        components_entity_end(&state->components, slayme);
        
        interacted = true;
    }
    
    return interacted;
}

typedef struct {
    bool herded;
    int32_t dx;
    int32_t dy;
} Herdment;

static Herdment command_move(State* state, Entity subject, int32_t tile_x, int32_t tile_y) {
    Herdment result = {false, 0, 0};
    
    if (subject == 0) {
        ERROR("Entity can't be 0.");
        return result;
    }
    if (!in_board(tile_x, tile_y)) {
        return result;
    }
    
    CPosition* position = component_of(&state->components.compgroups[COMPTYPE_POSITION], subject);
    if (position == NULL) {
        ERROR("Subject position component is missing.");
        return result;
    }

    /* Only move to adjacent square. */
    if (distance4(position->x, position->y, tile_x, tile_y) != 1) {
        return result;
    }
    position = NULL; /* The pointer can invalidate if components are removed so don't reuse. */

    bool interacted = interact(state, subject, tile_x, tile_y);
    if (!interacted && type_at(state, COMPTYPE_OBSTRUCTION, tile_x, tile_y) != 0) {
        return result;
    }

    /* Need to get position component again because the ECS can have been rearranged. */
    position = component_of(&state->components.compgroups[COMPTYPE_POSITION], subject);
    if (position == NULL) {
        ERROR("Subject position component is missing.");
        return result;
    }

    int32_t dx = tile_x - position->x;
    int32_t dy = tile_y - position->y;
    
    position->x = tile_x;
    position->y = tile_y;

    bool is_cd =
        (component_of(&state->components.compgroups[COMPTYPE_COOLDOWN], subject) != NULL);
    if (!is_cd) {
        /* TODO: Prevent the move from happening. */
        if (cooldown_init(&state->components, subject) == NULL) {
            ERROR("cooldown_init");
        }
    }

    bool is_herder =
        (component_of(&state->components.compgroups[COMPTYPE_HERDER], subject) != NULL);
    if (is_herder) {
        result.herded = true;
        result.dx = dx;
        result.dy = dy;
    }
    return result;
}

typedef struct {
    Entity entity;
    int32_t dest_x;
    int32_t dest_y;
} HerdMe;

void herd(State* state, int32_t dx, int32_t dy) {
    size_t max_herdmes = 10;
    HerdMe herdus[max_herdmes];
    memset(herdus, 0, max_herdmes * sizeof(HerdMe));
    size_t next_herdme = 0;

    CompGroup* groups[] = {
        &state->components.compgroups[COMPTYPE_FLOCK],
        &state->components.compgroups[COMPTYPE_POSITION],
    };
    void* comps[] = {NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 2)) {
        CPosition* position = (CPosition*)comps[1];

        herdus[next_herdme] = (HerdMe){position->entity, position->x + dx, position->y + dy};
        next_herdme += 1;
        if (next_herdme >= max_herdmes) {
            WARN("too many entities to herd");
            break;
        }        
    }
    
    for (size_t r = 0; r < max_herdmes; r += 1) {
        if (herdus[r].entity == 0) {
            break;
        }
        
        command_move(state, herdus[r].entity, herdus[r].dest_x, herdus[r].dest_y);
    }
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
            int32_t tile_x = x / TILE_SIZE;
            int32_t tile_y = y / TILE_SIZE;
            Herdment herdment = command_move(state, sel->subject, tile_x, tile_y);
            if (herdment.herded) {
                herd(state, herdment.dx, herdment.dy);
            }
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
