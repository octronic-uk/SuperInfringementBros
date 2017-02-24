#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL_image.h>

#include "vector.h"

typedef struct {
    SDL_Texture *texture;
    vector2f_t   velocity;
    vector2i_t   position;
    vector2i_t   dimensions;
    char scroll;
    char repeatScroll;
} background_t;

background_t* backgroundAllocate(char* path, SDL_Renderer*);
void backgroundDestroy(background_t*);

#endif // BACKGROUND_H
