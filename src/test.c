#ifdef TEST

#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

#include "logging.h"
#include "constants.h"
#include "state.h"
#include "icon.h"
#include "event.h"
#include "draw.h"
#include "terrain.h"

#include "minunit.h"

// typedef union {
    // Position position;
    // Avatar avatar;
// } ComponentState;
// 
// typedef struct {
    // Entity entity;
    // ComponentState state;
// } Component;

typedef struct {
    Entity entity;
} AbstractComp;

// #define sizeof(AbstractComp) no

typedef struct {
    Entity entity;
    int val;
} CompInt;

typedef struct {
    Entity entity;
    double val;
} CompDouble;



Entity entity_next(State* state) {
    
}

int component_init(State* state, Entity entity) {
    
}





// void entity_end(State* state, Entity entity) {
    // bool found = false;
    // for (int i = 0; i < MAX_COMPONENTS; i += 1) {
        // Avatar* comp = state->avatars[i];
        // if (found) {
            // /* Safe because found will never == true on first iteration. */
            // state->avatars[i - 1] = *comp;
            // if (comp->entity == 0) {
                // break;
            // }
            // comp->entity = 0;
        // } else {
            // if (comp->entity == 0) {
                // break;
            // }
            // if (comp->entity == entity) {
                // found = true;
            // }
        // }
    // }
// }

#include <stdint.h>


AbstractComp* component_at(void* components, uintptr_t compsize, uintptr_t index) {
    return (AbstractComp*)(components + (index * compsize));
}


void component_end(void* components, uintptr_t len, uintptr_t compsize, Entity entity) {
    bool found = false;
    for (uintptr_t index = 0; index < len; index += 1) {
        AbstractComp* comp = component_at(components, compsize, index);
        if (found) {
            /* memcpy is safe because found will never == true on first iteration. */
            void* dest = components + (index - 1) * compsize;
            void* source = components + (index * compsize);
            memcpy(dest, source, compsize);
            
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
}

// void draw(state) {
    // for (int i = 0; i < MAX_COMPONENTS; i += 1) {
        // // check if entity alive
        // CPosition* cposition = ...
        // CIcon* cicon = ...
// 
        // draw(cposition, cicon);
    // }
// }


int tests_run = 0;
int tests_failed = 0;

static char* test_compbgone() {
    CompInt comps[3];
    comps[0].entity = 1;
    comps[0].val = 6;
    
    comps[1].entity = 2;
    comps[1].val = 5;
    
    comps[2].entity = 3;
    comps[2].val = 4;

    component_end(&comps, 3, sizeof(CompInt), 1);
    mu_assert(comps[0].entity == 2, "");
    mu_assert(comps[0].val == 5, "");
    mu_assert(comps[1].entity == 3, "");
    mu_assert(comps[1].val == 4, "");
    mu_assert(comps[2].entity == 0, "");
    return 0;
}

int main(int argc, char **argv) {
    mu_run_test(test_compbgone);

    printf("Passed: %d Failed: %d\n", tests_run - tests_failed, tests_failed);

    return tests_failed;
}

#endif /* TEST */
