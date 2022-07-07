#include "SDL.h"
#include "entity.h"
#include "constants.h"

Components components_new() {
    Components result;
    result.compgroups[COMPTYPE_POSITION] = compgroup_init(60, sizeof(CPosition));
    result.compgroups[COMPTYPE_AVATAR] = compgroup_init(10, sizeof(CAvatar));
    result.compgroups[COMPTYPE_SELECTABLE] = compgroup_init(10, sizeof(CSelectable));
    result.compgroups[COMPTYPE_MOUNT] = compgroup_init(1, sizeof(CMount));
    result.compgroups[COMPTYPE_RIDER] = compgroup_init(1, sizeof(CRider));
    result.compgroups[COMPTYPE_MUNCH] = compgroup_init(1, sizeof(CMunch));
    result.compgroups[COMPTYPE_EDIBLE] = compgroup_init(10, sizeof(CEdible));
    result.compgroups[COMPTYPE_SLAYER] = compgroup_init(1, sizeof(CSlayer));
    result.compgroups[COMPTYPE_SLAYME] = compgroup_init(1, sizeof(CSlayMe));
    result.compgroups[COMPTYPE_OBSTRUCTION] = compgroup_init(60, sizeof(CObstruction));
    result.compgroups[COMPTYPE_TILE] = compgroup_init(60, sizeof(CTile));
    result.compgroups[COMPTYPE_HERDER] = compgroup_init(1, sizeof(CHerder));
    result.compgroups[COMPTYPE_FLOCK] = compgroup_init(10, sizeof(CFlock));
    result.compgroups[COMPTYPE_COOLDOWN] = compgroup_init(10, sizeof(CCooldown));
    result.compgroups[COMPTYPE_TWEEN] = compgroup_init(10, sizeof(CCooldown));
    return result;
}

Entity type_at(State* state, uint8_t comptype, Coord tile_x, Coord tile_y) {
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

void components_entity_end(Components* comps, Entity entity) {
    compgroups_entity_end(comps->compgroups, COMPTYPE_COUNT, entity);
}

void components_clear(Components* comps) {
    for (size_t r = 0; r < COMPTYPE_COUNT; r += 1) {
        compgroup_clear(&comps->compgroups[r]);
    }
}

CPosition* position_init(Components* components, Entity entity, Coord x, Coord y) {
    CompGroup* group = &components->compgroups[COMPTYPE_POSITION];
    CPosition* result = (CPosition*)component_init(group, entity);
    if (result != NULL) {
        result->x = x;
        result->y = y;
    }
    return result;
}

CAvatar* avatar_init(Components* components, Entity entity, IconID icon_id, float_t x, float_t y) {
    CompGroup* group = &components->compgroups[COMPTYPE_AVATAR];
    CAvatar* result = (CAvatar*)component_init(group, entity);
    if (result != NULL) {
        result->icon_id = icon_id;
        result->x = x;
        result->y = y;
    }
    return result;
}

void* empty_init(Components* components, uint8_t comptype, Entity entity) {
    CompGroup* group = &components->compgroups[comptype];
    return component_init(group, entity);
}

CSelectable* selectable_init(Components* components, Entity entity) {
    return (CSelectable*)empty_init(components, COMPTYPE_SELECTABLE, entity);
}

CMount* mount_init(Components* components, Entity entity) {
    return (CMount*)empty_init(components, COMPTYPE_MOUNT, entity);
}

CRider* rider_init(Components* components, Entity entity) {
    return (CRider*)empty_init(components, COMPTYPE_RIDER, entity);
}

CMunch* munch_init(Components* components, Entity entity) {
    return (CMunch*)empty_init(components, COMPTYPE_MUNCH, entity);
}

CEdible* edible_init(Components* components, Entity entity) {
    return (CEdible*)empty_init(components, COMPTYPE_EDIBLE, entity);
}

CSlayer* slayer_init(Components* components, Entity entity) {
    return (CSlayer*)empty_init(components, COMPTYPE_SLAYER, entity);
}

CSlayMe* slayme_init(Components* components, Entity entity) {
    return (CSlayMe*)empty_init(components, COMPTYPE_SLAYME, entity);
}

CObstruction* obstruction_init(Components* components, Entity entity) {
    return (CObstruction*)empty_init(components, COMPTYPE_OBSTRUCTION, entity);
}

CTile* tile_init(Components* components, Entity entity, IconID icon_id) {
    CompGroup* group = &components->compgroups[COMPTYPE_TILE];
    CTile* result = (CTile*)component_init(group, entity);
    if (result != NULL) {
        result->icon_id = icon_id;
    }
    return result;
}

CHerder* herder_init(Components* components, Entity entity) {
    return (CHerder*)empty_init(components, COMPTYPE_HERDER, entity);
}

CFlock* flock_init(Components* components, Entity entity) {
    return (CFlock*)empty_init(components, COMPTYPE_FLOCK, entity);
}

CCooldown* cooldown_init(Components* components, Entity entity) {
    return (CCooldown*)empty_init(components, COMPTYPE_COOLDOWN, entity);
}

CTween* tween_init(Components* components, Entity entity) {
    return (CTween*)empty_init(components, COMPTYPE_TWEEN, entity);
}
