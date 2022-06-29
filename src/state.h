
typedef struct {
    Entity entity;
    int x;
    int y;
} CPosition;

typedef struct {
    Entity entity;
    Icon icon;
} CAvatar;

/*
    Allocates and returns a new blank State. Caller must free the state. Returns NULL if the
    allocation failed.
 */
State* make_state();

/*
    Deallocates and zeroes out the state.
 */
void destroy_state(State* state);
