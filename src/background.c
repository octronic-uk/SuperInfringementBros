#include "background.h"


background_t* backgroundAllocate(char* path, const SDL_PixelFormat *format) {
   background_t *bg = (background_t*)malloc(sizeof(background_t)); 
   SDL_Surface *tmp = IMG_Load(path);
   bg->surface = SDL_ConvertSurface(tmp,format,0);
   SDL_FreeSurface(tmp);
   bg->vel.x = 0;
   bg->vel.y = 0;
   bg->pos.x = 0;
   bg->pos.y = 0;
   return bg;
}

void backgroundDestroy(background_t* self) {
    if (self != NULL) {
        if (self->surface != NULL) {
            SDL_FreeSurface(self->surface);
            self->surface = NULL;
        }
    }
}

