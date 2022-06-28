#include <stdbool.h>

#define TILE_SIZE 64
#define TILES_ACROSS 10
#define TILES_DOWN 6
#define TILE_RIGHT (TILES_ACROSS - 1)
#define TILE_BOTTOM (TILES_DOWN - 1)

#define VIEW_WIDTH (TILE_SIZE * TILES_ACROSS)
#define VIEW_HEIGHT (TILE_SIZE * TILES_DOWN)

#define TEXTURE_UNKNOWN -1
#define TEXTURE_TILES 0
#define TEXTURE_DRAGON 1
#define TEXTURE_KNIGHT 2
#define TEXTURE_SHEEP 3
#define TEXTURE_TERRAIN 4
#define TEXTURE_COUNT 5

/* Putting type definitions here to resolve issues with circular imports. */

typedef int TexID;

typedef struct {
    TexID texture_id;
    SDL_Rect source_rect;
} Icon;

typedef struct {
    int hover_x;
    int hover_y;
    int select_x;
    int select_y;
} Selection;

/* 0 = no entity */
typedef int Entity;

// #define MAX_COMPONENTS 20

typedef struct {
    Entity entity;
    int x;
    int y;
} Position;

typedef struct {
    Entity entity;
    Icon icon;
} Avatar;

typedef struct {
    bool needs_redraw;
    Selection selection;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[TEXTURE_COUNT];
    Icon icon_wall;
    Icon icon_floor;
    Icon icon_pyramid;

    // Position positions[MAX_COMPONENTS];
    // Avatar avatars[MAX_COMPONENTS];
} State;
