
void command_move(State* state, Entity subject, Coord tile_x, Coord tile_y);

bool can_reach(Coord ax, Coord ay, Coord bx, Coord by);

bool will_interact(State* state, Entity subject, Coord tile_x, Coord tile_y);
