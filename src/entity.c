#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include <stddef.h>
#include "logging.h"
#include "constants.h"
#include "entity.h"

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

// Entity entity_next(State* state) {
    // 
// }
// 
// int component_init(State* state, Entity entity) {
    // 
// }



AbstractComp* component_at(void* mem, size_t compsize, size_t index) {
    return (AbstractComp*)(mem + (index * compsize));
}

void component_end(CompGroup* components, Entity entity) {
    bool found = false;
    for (size_t index = 0; index < components->alive; index += 1) {
        AbstractComp* comp = component_at(components->mem, components->compsize, index);
        if (found) {
            /* memcpy is safe because found will never == true on first iteration. */
            void* dest = components->mem + (index - 1) * components->compsize;
            void* source = components->mem + (index * components->compsize);
            memcpy(dest, source, components->compsize);
            
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
        components->alive -= 1;
    }
}

void groups_entity_end(CompGroup* groups, size_t ngroups, Entity entity) {
    for (size_t i = 0; i < ngroups ; i += 1) {
        CompGroup* group = &groups[i];
        component_end(group, entity);
    }
}
