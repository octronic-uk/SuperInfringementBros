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
    int currentFrame;
    vector2i_t *frameOrder;
} sprite_t;

sprite_t* spriteAllocate(char* path,SDL_Renderer* renderer);
sprite_t* spriteAllocateSpriteSheet(char* path, int tileWidth, int tileHeight, int animFrames, SDL_Renderer* renderer);
void spriteDestroy(sprite_t* self);
void spriteAdvanceFrame(sprite_t* self);
SDL_Rect spriteGetCurrentFrameRect(sprite_t* self);

#endif // SPRITE_H
