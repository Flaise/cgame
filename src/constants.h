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
#define TEXTURE_MKNIGHT 5
#define TEXTURE_DOG 6
#define TEXTURE_HORSE 7
#define TEXTURE_COUNT 8

/* Putting type definitions here to resolve issues with circular imports. */

typedef int32_t TexID;

typedef struct {
    TexID texture_id;
    SDL_Rect source_rect;
} Icon;

typedef struct {
    int32_t hover_x;
    int32_t hover_y;
    int32_t select_x;
    int32_t select_y;
    Entity subject;
    bool hover_valid;
} Selection;

#define COMPTYPE_POSITION 0
#define COMPTYPE_AVATAR 1
#define COMPTYPE_SELECTABLE 2
#define COMPTYPE_MOUNT 3
#define COMPTYPE_RIDER 4
#define COMPTYPE_MUNCH 5
#define COMPTYPE_EDIBLE 6
#define COMPTYPE_SLAYER 7
#define COMPTYPE_SLAYME 8
#define COMPTYPE_OBSTRUCTION 9
#define COMPTYPE_TILE 10
#define COMPTYPE_HERDER 11
#define COMPTYPE_FLOCK 12
#define COMPTYPE_COUNT 13

typedef struct {
    Entity entity;
    int x;
    int y;
} CPosition;

typedef struct {
    Entity entity;
    Icon icon; /* TODO: Use icon_id and lookup table instead of using copies/pointers. */
} CAvatar, CTile;

typedef struct {
    Entity entity;
} CSelectable, CMount, CRider, CMunch, CEdible, CSlayer, CSlayMe, CObstruction, CHerder, CFlock;

typedef struct {
    CompGroup compgroups[COMPTYPE_COUNT];
} Components;

typedef struct {
    bool needs_redraw;
    
    Selection selection;
    Components components;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[TEXTURE_COUNT];

    /* TODO: Icon icons[ICON_COUNT]; */
    Icon icon_wall;
    Icon icon_floor_a;
    Icon icon_floor_b;
    Icon icon_pyramid;
    Icon icon_dragon;
    Icon icon_knight;
    Icon icon_mknight;
    Icon icon_sheep;
    Icon icon_dog;
    Icon icon_horse;
} State;
