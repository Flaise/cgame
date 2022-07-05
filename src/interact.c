#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"
#include "board.h"

static int32_t distance4(int32_t ax, int32_t ay, int32_t bx, int32_t by) {
    return abs(ax - bx) + abs(ay - by);
}

bool can_reach(Coord ax, Coord ay, Coord bx, Coord by) {
    return distance4(ax, ay, bx, by) == 1;
}

bool will_interact(State* state, Entity subject, Coord tile_x, Coord tile_y) {
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

static bool interact(State* state, Entity subject, Coord tile_x, Coord tile_y) {
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

static Herdment do_move(State* state, Entity subject, Coord tile_x, Coord tile_y) {
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

    Coord dx = tile_x - position->x;
    Coord dy = tile_y - position->y;
    
    position->x = tile_x;
    position->y = tile_y;

    bool is_selectable =
        (component_of(&state->components.compgroups[COMPTYPE_SELECTABLE], subject) != NULL);
    if (is_selectable) {
        /* Go on cooldown. */
        if (cooldown_init(&state->components, subject) == NULL) {
            ERROR("cooldown_init");
        }

        /* Clear cooldowns when last piece moves. */
        if (state->components.compgroups[COMPTYPE_COOLDOWN].alive
        >= state->components.compgroups[COMPTYPE_SELECTABLE].alive) {
            compgroup_clear(&state->components.compgroups[COMPTYPE_COOLDOWN]);
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
        
        do_move(state, herdus[r].entity, herdus[r].dest_x, herdus[r].dest_y);
    }
}

void command_move(State* state, Entity subject, Coord tile_x, Coord tile_y) {
    if (subject == 0) {
        WARN("subject can't be 0.");
        return;
    }
    
    Herdment herdment = do_move(state, subject, tile_x, tile_y);
    if (herdment.herded) {
        herd(state, herdment.dx, herdment.dy);
    }
}
