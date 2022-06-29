
/*
 Creates a new Components object.
 */
Components make_components();

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CPosition* position_init(Components* components, Entity entity, int x, int y);

/*
 Returns: A pointer to the newly initialized component or NULL if out of memory.
 */
CAvatar* avatar_init(Components* components, Entity entity, Icon icon);
