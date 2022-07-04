#include <stddef.h>
#include <stdbool.h>

/* 0 = no entity */
typedef uint32_t Entity;

typedef struct {
    Entity entity;
} AbstractComp;

typedef struct {
    void* mem;
    uint32_t alive;
    uint32_t total;
    size_t compsize;
} CompGroup;

/*
 Constructs a new component group.
 total: the number of components that can be stored in the group
 compsize: the size in bytes of the component type stored in the group
 */
CompGroup compgroup_init(uint32_t total, size_t compsize);

/*
 Removes every component in the group.
 */
void compgroup_clear(CompGroup* group);

/*
 Allocates a new component in the component group.
 Returns: A borrowed reference to the new component or NULL if the group is out of memory.
 */
void* component_init(CompGroup* group, Entity entity);

/*
 Removes the component attached to the specified entity if it exists.
 */
void component_end(CompGroup* group, Entity entity);

void* component_of(CompGroup* group, Entity entity);

/*
 Removes all components attached to the specified entity, if any exist.
 */
void compgroups_entity_end(CompGroup* group_arr, int8_t ngroups, Entity entity);

/*
 Successive calls yield components that share an entity. Skips over entities that don't have all of
 the given component types.

 Usage:
    CompGroup groupa = compgroup_init(5, sizeof(ComponentA));
    CompGroup groupb = compgroup_init(5, sizeof(ComponentB));

    CompGroup* groups[] = {&groupa, &groupb};
    void* comps[] = {NULL, NULL};
    while (component_iterate((CompGroup**)&groups, (void**)&comps, 2)) {
        ComponentA* a = comps[0];
        ComponentB* b = comps[1];

        // do something with a and b
    }

 groups: Pointer to an array of pointers to CompGroup objects.
 comps: Pointer to an array of pointers to components. The pointers in this array should be
    initialized to NULL when beginning iteration. After each call to component_iterate() returns,
    they will be updated to point at the components of the next entity.
 ncomps: The number of components and component groups to iterate. The pointer of each component in
    comps corresponds to the pointer at the same index of groups. Each array should be the same
    length or at least be as long or longer than ncomps.
 Returns: true if the next entity was found and the pointers in comps were updated, or false if
    iteration is done.
 */
bool component_iterate(CompGroup** groups, void** comps, int8_t ncomps);
