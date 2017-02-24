#include "text.h"
#include "logger.h"

text_t *textAllocate(char* fontPath, int size, int bufferSize) {
    debug("Allocating text_t from %s, size %d\n",fontPath,size);
    text_t *text = (text_t*)malloc(sizeof(text_t));

    text->font = TTF_OpenFont(fontPath,size);
    if (text->font == NULL) {
        error("Unable to open font %s. %s\n",fontPath,TTF_GetError());
        free(text);
        return NULL;
    }

    text->bufferSize = bufferSize;
    text->text = (char*)malloc(sizeof(char)*text->bufferSize);
    memset(text->text,0,text->bufferSize);

    text->position.x = 0;
    text->position.y = 0;

    text->colour.r = 255;
    text->colour.g = 255;
    text->colour.b = 255;
    text->colour.a = 255;
    return text;
}

void textDestroy(text_t* self) {
    debug("Destroying text_t\n");
    if (self != NULL) {
        if (self->font != NULL) {
            TTF_CloseFont(self->font);
            self->font = NULL;
        }
        if(self->text != NULL) {
            free (self->text);
            self->text = NULL;
        }
        free (self);
    }
}


