#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL_image.h>
#include "vector.h"

typedef struct {
    SDL_Texture* texture;
    vector2i_t   dimensions;
    vector2i_t   tileDimensions;
    // Animation
    int numFrames;
    vector2i_t *frameOrder;
} sprite_t;

sprite_t* spriteAllocate(char*,SDL_Renderer*);
sprite_t* spriteAllocateSpriteSheet(char*, int, int, int, SDL_Renderer*);
void spriteDestroy(sprite_t*);

#endif // SPRITE_H
