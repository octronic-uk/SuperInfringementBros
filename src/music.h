#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_Mixer.h>

typedef struct {
    Mix_Music *music;
    int loops;
} music_t;

music_t* musicAllocate(char* audioFile, int loops);
void musicDestroy(music_t* self);
void musicPlay(music_t* self);

#endif //MUSIC_H
