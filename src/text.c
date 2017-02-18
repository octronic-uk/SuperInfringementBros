#include "text.h"
#include <stdio.h>

text_t *textAllocate(char* fontPath, char* str, int size) {
    printf("Allocating text_t from %s, size %d\n",fontPath,size);
    text_t *text = (text_t*)malloc(sizeof(text_t));

    text->font = TTF_OpenFont(fontPath,size);
    text->text = str;

    text->position.x = 0;
    text->position.y = 0;

    text->colour.r = 255;
    text->colour.g = 255;
    text->colour.b = 255;
    text->colour.a = 255;
    return text;
}

void textDestroy(text_t* self) {
    printf("Destroying text_t\n");
    if (self != NULL) {
        if (self->font != NULL) {
            TTF_CloseFont(self->font);
            self->font = NULL;
        }
        free (self);
    }
}


