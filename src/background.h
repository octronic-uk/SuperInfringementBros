#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "vector.h"
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Surface *surface;
    vector2i_t vel;
    vector2i_t pos;
} background_t;

background_t* backgroundAllocate(char* path, const SDL_PixelFormat*);
void backgroundDestroy(background_t*);

#endif // BACKGROUND_H
