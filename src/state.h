
/*
    Allocates and returns a new blank State. Caller must free the state. Returns NULL if the
    allocation failed.
 */
State* state_new();

/*
    Deallocates the contents of the state and zeroes out the pointers.
 */
void state_end(State* state);
