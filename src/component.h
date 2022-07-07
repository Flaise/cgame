
/*
 Creates a new Components object.
 */
Components components_new();

/*
 Returns: The entity that has a position component that matches the given tile_x and tile_y, or
          0 if there is no such entity.
 */
Entity type_at(State* state, uint8_t comptype, Coord tile_x, Coord tile_y);

void components_entity_end(Components* comps, Entity entity);

void components_clear(Components* comps);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CPosition* position_init(Components* components, Entity entity, Coord x, Coord y);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CAvatar* avatar_init(Components* components, Entity entity, IconID icon_id, float_t x, float_t y);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CSelectable* selectable_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CMount* mount_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CRider* rider_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CMunch* munch_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CEdible* edible_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CSlayer* slayer_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CSlayMe* slayme_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CObstruction* obstruction_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CTile* tile_init(Components* components, Entity entity, IconID icon_id);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CHerder* herder_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CFlock* flock_init(Components* components, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CCooldown* cooldown_init(Components* components, Entity entity);
