#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL_ttf.h>

#include "vector.h"

typedef struct text text_t;

struct text {
    vector2i_t position;
    SDL_Color  colour;
    char      *text;
    int        bufferSize;
    TTF_Font  *font;
    void (*updateFunction )(text_t* enemy, void *vEngine);
};

text_t *textAllocate(char *fontPath, int size, int bufferSize);
void textDestroy(text_t* self);

#endif // TEXT_H
