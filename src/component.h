
/*
 Creates a new Components object.
 */
Components make_components();

void components_entity_end(Components* comps, Entity entity);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CPosition* position_init(Components* components, Entity entity, int x, int y);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CAvatar* avatar_init(Components* components, Entity entity, Icon icon);

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
