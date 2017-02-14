#include <stdlib.h>
#include <stdio.h>
#include "sprite.h"


sprite_t* spriteAllocateSpriteSheet(char* path, int tileWidth, int tileHeight, int animFrames, const SDL_PixelFormat* screenFormat) {
    printf("Allocating new sprite_t\n");
    // Object
    sprite_t *s = (sprite_t*)malloc(sizeof(sprite_t));
    // Image Surface
    SDL_Surface* tmp  = IMG_Load(path);
    s->surface = SDL_ConvertSurface(tmp,screenFormat,0); 
    SDL_FreeSurface(tmp);
    // Source Rectangle
    s->dimensions.x = tileWidth;
    s->dimensions.y = tileHeight;
    // Frames
    if (animFrames > 0) {
        s->numFrames = animFrames;
        s->frameOrder = (vector2i_t*)malloc(sizeof(vector2i_t)*animFrames);
    } else {
        s->numFrames = 0;
        s->frameOrder = NULL; 
    }
    return s;
}

void spriteDestroy(sprite_t *self) {
    printf("Destroying sprite_t\n");
    if (self != NULL) {
        // Source Texture
        if (self->surface != NULL) {
            SDL_FreeSurface(self->surface);
            self->surface = NULL;
        }
        // Anim Frames
        if (self->frameOrder != NULL) {
            free(self->frameOrder);
            self->frameOrder = NULL;
            self->numFrames = 0;
        }
        // Object itself
        free (self);
        self = NULL;
    }
}
