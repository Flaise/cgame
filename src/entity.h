#include <stddef.h>
#include <stdbool.h>

/* 0 = no entity */
typedef uint32_t Entity;

typedef struct {
    Entity entity;
} AbstractComp;

typedef struct {
    void* mem;
    size_t alive;
    size_t total;
    size_t compsize;
} CompGroup;

/*
 Constructs a new component group.
 total: the number of components that can be stored in the group
 compsize: the size in bytes of the component type stored in the group
 */
CompGroup compgroup_init(size_t total, size_t compsize);

/*
 Allocates a new component in the component group.
 Returns: An borrowing pointer to the new component or NULL if the group is out of memory.
 */
void* component_init(CompGroup* group, Entity entity);

/*
 Removes the component attached to the specified entity if it exists.
 */
void component_end(CompGroup* group, Entity entity);

/*
 Removes all components attached to the specified entity, if any exist.
 */
void compgroups_entity_end(CompGroup* group_arr, size_t ngroups, Entity entity);

bool component_iterate(CompGroup** groups, void** comps, int8_t ncomps);
