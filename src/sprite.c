#include <stdlib.h>
#include <stdio.h>
#include "sprite.h"

sprite_t* spriteAllocate(char* path, SDL_Renderer* renderer) {
    printf("Allocating new sprite_t from %s\n",path);
    // Object
    sprite_t *s = (sprite_t*)malloc(sizeof(sprite_t));
    // Image Surface
    s->texture = IMG_LoadTexture(renderer, path);
    // Dimensions
    SDL_QueryTexture(s->texture,NULL,NULL,&(s->dimensions.x),&(s->dimensions.y));
    // Source Rectangle
    s->tileDimensions.x = 0;
    s->tileDimensions.y = 0;
    // Frames
    s->numFrames = 0;
    s->frameOrder = NULL; 
    return s;
}


sprite_t* spriteAllocateSpriteSheet(char* path, int tileWidth, int tileHeight, int animFrames, SDL_Renderer* renderer) {
    // Sprite Obj
    sprite_t *s = spriteAllocate(path,renderer);
    // Tile Dimensions
    s->tileDimensions.x = tileWidth;
    s->tileDimensions.y = tileHeight;
    s->currentFrame = 0;
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

void spriteAdvanceFrame(sprite_t* self) {
    self->currentFrame++;
    if (self->currentFrame > self->numFrames) {
        self->currentFrame = 0;
    }
}

void spriteDestroy(sprite_t *self) {
    printf("Destroying sprite_t\n");
    if (self != NULL) {
        // Source Texture
        if (self->texture != NULL) {
            SDL_DestroyTexture(self->texture);
            self->texture = NULL;
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

SDL_Rect spriteGetCurrentFrameRect(sprite_t* self) {
    SDL_Rect frameRect;
    frameRect.w = self->tileDimensions.x;
    frameRect.h = self->tileDimensions.y;
    frameRect.x = self->frameOrder[self->currentFrame].x*self->tileDimensions.x;
    frameRect.y = self->frameOrder[self->currentFrame].y*self->tileDimensions.y;
    return frameRect;
}
