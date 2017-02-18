#ifndef SFX_H
#define SFX_H

#include <SDL2/SDL_Mixer.h>

typedef struct {
    Mix_Chunk *chunk;
    int loops;
} sfx_t;

sfx_t* sfxAllocate(char* audioFile, int loops);
void sfxDestroy(sfx_t* self);
void sfxPlay(sfx_t* self);

#endif // SFX_H
