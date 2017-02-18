#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL_ttf.h>
#include "vector.h"

typedef struct {
    vector2i_t position;
    SDL_Color  colour;
    char      *text;
    TTF_Font  *font;
} text_t;

text_t *textAllocate(char *fontPath, char *str, int size);
void textDestroy(text_t* self);

#endif // TEXT_H
