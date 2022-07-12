#include "SDL.h"
#include "logging.h"
#include "entity.h"
#include "constants.h"

#include "res/music.h"
#define RES_MUSIC __res_sinister_abode_ogg

void audio_done_blocking(State* state) {
    /* Mix_Init(0) will return the incorrect value if it reported a false failure upon startup. */
    /* if (Mix_Init(0) == 0) {
        return;
    } */
    
    if (Mix_FadeOutMusic(2000) != 0) {
        ERROR("Mix_FadeOutMusic");
    }

    /* Necessary because Mix_FreeMusic doesn't block correctly if Mix_Init falsely indicated failure
       upon completion. */
    SDL_Delay(2000);
    
    Mix_FreeMusic(state->music); /* Blocks until fade-out completes. */
    Mix_CloseAudio();
    Mix_Quit();
    
    state->music = NULL;
}

static Mix_Music* const_ogg_to_music(const void* mem, int size) {
    SDL_RWops* rw = SDL_RWFromConstMem(mem, size);
    if (rw == NULL) {
        return NULL;
    }
    
    return Mix_LoadMUSType_RW(rw, MUS_OGG, true);
}

int audio_init(State* state) {
    int flags = Mix_Init(MIX_INIT_OGG);
    if ((flags & MIX_INIT_OGG) == 0) {
        WARN("Mix_Init returned %d", flags);

        /* On Ubuntu it's giving a false failure. The audio loads and plays fine. */
        /* I hate computers. */
    }
    
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        WARN("Mix_OpenAudio");
        
        /* This can be an intermittent error or a missing driver. Keep going without audio. */
        return 0;
    }

    Mix_Music* music = const_ogg_to_music(RES_MUSIC, sizeof(RES_MUSIC));
    if (music == NULL) {
        ERROR("Couldn't load music.");
        return 1;
    }

    if (Mix_FadeInMusic(music, -1, 4000) != 0) {
        ERROR("Mix_FadeInMusic");
        return 1;
    }

    return 0;
}
