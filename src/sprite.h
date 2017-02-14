#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL_image.h>
#include "vector.h"

typedef struct {
    SDL_Surface *surface;
    vector2i_t   dimensions;
    // Animation
    int numFrames;
    vector2i_t *frameOrder;
} sprite_t;

sprite_t* spriteAllocateSpriteSheet(char*, int, int, int, const SDL_PixelFormat*);
void spriteDestroy(sprite_t*);

#endif // SPRITE_H
