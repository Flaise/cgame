
typedef struct {
    Entity entity;
} AbstractComp;

typedef struct {
    void* mem;
    size_t alive;
    size_t total;
    size_t compsize;
} CompGroup;

void* component_alloc(CompGroup* group, Entity entity);
CompGroup compgroup_init(size_t total, size_t compsize);
void component_end(CompGroup* group, Entity entity);
void groups_entity_end(CompGroup* group_arr, size_t ngroups, Entity entity);
