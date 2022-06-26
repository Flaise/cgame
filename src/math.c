#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif
#include "SDL_image.h"

SDL_Rect letterbox(int container_w, int container_h, int tex_w, int tex_h) {

    /* TODO: finish this algorithm */

    SDL_Rect result = {
        .x = container_w / 2 - tex_w / 2,
        .y = container_h / 2 - tex_h / 2,
        .w = tex_w,
        .h = tex_h,
    };
    return result;
}
