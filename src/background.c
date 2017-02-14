#include "background.h"
#include <stdio.h>


background_t* backgroundAllocate(char* path, SDL_Renderer *renderer) {

    if (renderer == NULL) {
        printf("Unable to load background_t image, format is null\n");
        return NULL;
    }

    printf("Allocating new background_t from %s\n",path);

    background_t *bg = (background_t*)malloc(sizeof(background_t)); 
    bg->texture = IMG_LoadTexture(renderer, path);

    if (bg->texture == NULL) {
        printf("Unable to load texture for background_t %s\n",path);
        free(bg);
        return NULL;
    }

    bg->velocity.x = 0;
    bg->velocity.y = 0;
    bg->position.x = 0;
    bg->position.y = 0;

    SDL_QueryTexture(bg->texture,NULL,NULL,&(bg->dimensions.x),&(bg->dimensions.y));

    return bg;
}

void backgroundDestroy(background_t* self) {
    printf("Destroying background_t\n");
    if (self != NULL) {
        if (self->texture != NULL) {
            SDL_DestroyTexture(self->texture);
            self->texture = NULL;
        }
    }
}

