#include <stdbool.h>
#include <stdint.h>
#include <math.h>

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
#define TEXTURE_COOLDOWN 8
#define TEXTURE_INSTRUCTIONS 9
#define TEXTURE_COUNT 10

#define ICON_WALL 0
#define ICON_FLOOR_A 1
#define ICON_FLOOR_B 2
#define ICON_PYRAMID 3
#define ICON_DRAGON 4
#define ICON_KNIGHT 5
#define ICON_MKNIGHT 6
#define ICON_SHEEP 7
#define ICON_DOG 8
#define ICON_HORSE 9
#define ICON_COOLDOWN 10
#define ICON_COUNT 11

/* Putting type definitions here to resolve issues with circular imports. */

typedef int32_t TexID;
typedef uint8_t IconID;
typedef int16_t Coord;
typedef uint8_t LevelID;

typedef struct {
    TexID texture_id;
    SDL_Rect source_rect;
} Icon;

typedef enum {
    HoverEmpty,
    HoverValid,
    HoverInvalid,
} HoverStatus;

typedef struct {
    Coord hover_x;
    Coord hover_y;
    Coord select_x;
    Coord select_y;
    Entity subject;
    HoverStatus hover_status;
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
#define COMPTYPE_COOLDOWN 13
#define COMPTYPE_TWEEN 14
#define COMPTYPE_COUNT 15

typedef struct {
    Entity entity;
    Coord x;
    Coord y;
} CPosition;

typedef struct {
    Entity entity;
    IconID icon_id;
    float_t x;
    float_t y;
} CAvatar;

typedef struct {
    Entity entity;
    IconID icon_id;
} CTile;

typedef struct {
    Entity entity;
} CSelectable, CMount, CRider, CMunch, CEdible, CSlayer, CSlayMe, CObstruction, CHerder, CFlock,
    CCooldown, CTween;

typedef struct {
    CompGroup compgroups[COMPTYPE_COUNT];
} Components;

typedef struct {
    Selection selection;
    Components components;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[TEXTURE_COUNT];
    bool needs_redraw;
    
    Icon icons[ICON_COUNT];

    LevelID level_id;

    bool exiting;
} State;
