#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include <stddef.h>
#include "logging.h"
#include "entity.h"
#include "constants.h"
#include "component.h"

CompGroup compgroup_init(size_t total, size_t compsize) {
    if (total == 0 || compsize == 0) {
        ERROR("Invalid total or component size.");
    }
    CompGroup result;
    result.mem = calloc(total, compsize);
    result.alive = 0;
    result.total = total;
    result.compsize = compsize;
    return result;
}

static AbstractComp* component_at(void* mem, size_t compsize, size_t index) {
    return (AbstractComp*)(mem + (index * compsize));
}

void* component_init(CompGroup* group, Entity entity) {
    if (group == NULL) {
        return NULL;
    }
    if (group->alive == group->total) {
        return NULL;
    }
    if (entity == 0) {
        WARN("Entity can't be 0.");
        return NULL;
    }

    /* TODO: also do insertion sort here */
    for (size_t r = group->alive; r > 0;) {
        /* Loop counter has to decrement first because the size_t can't go negative. */
        r -= 1;
        
        AbstractComp* other = component_at(group->mem, group->compsize, r);
        if (other->entity == entity) {
            return NULL;
        }
    }
    
    AbstractComp* result = component_at(group->mem, group->compsize, group->alive);
    result->entity = entity;
    group->alive += 1;
    return (void*)result;
}

void component_end(CompGroup* group, Entity entity) {
    bool found = false;
    for (size_t index = 0; index < group->alive; index += 1) {
        AbstractComp* comp = component_at(group->mem, group->compsize, index);
        if (found) {
            /* memcpy is safe because found will never == true on first iteration. */
            void* dest = group->mem + (index - 1) * group->compsize;
            void* source = group->mem + (index * group->compsize);
            memcpy(dest, source, group->compsize);
            
            if (comp->entity == 0) {
                break;
            }
            comp->entity = 0; /* Necessary to clear component on last iteration. */
        } else {
            if (comp->entity == 0) {
                break;
            }
            if (comp->entity == entity) {
                found = true;
            }
        }
    }
    if (found) {
        group->alive -= 1;
    }
}

void groups_entity_end(CompGroup* group_arr, size_t ngroups, Entity entity) {
    for (size_t i = 0; i < ngroups ; i += 1) {
        CompGroup* group = &group_arr[i];
        component_end(group, entity);
    }
}
