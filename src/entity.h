
typedef struct {
    Entity entity;
} AbstractComp;

typedef struct {
    void* mem;
    size_t alive;
    size_t total;
    size_t compsize;
} CompGroup;

CompGroup compgroup_init(size_t total, size_t compsize);
void component_end(CompGroup* components, Entity entity);
void groups_entity_end(CompGroup* groups, size_t ngroups, Entity entity);
