#include "sfx.h"
#include "logger.h"

sfx_t* sfxAllocate(char* audioFile, int loops) {
    debug("Allocating sfx_t from %s loop=%d\n",audioFile,loops);
    sfx_t *sfx = (sfx_t*)malloc(sizeof(sfx_t));
    sfx->chunk = Mix_LoadWAV(audioFile);
    if (sfx->chunk == NULL) {
        debug("Error loading sfx_t %s\n",audioFile);
        free(sfx);
        return NULL;
    }
    sfx->loops = loops;
    return sfx;
}

void sfxDestroy(sfx_t* self) {
    debug("Destroying sfx_t\n");
    if (self != NULL) {
        if(self->chunk != NULL) {
            Mix_FreeChunk(self->chunk);
            self->chunk = NULL;
        }
        free(self);
    }
}

void sfxPlay(sfx_t* self) {
    if(Mix_PlayChannel(-1, self->chunk, self->loops) == -1) {
        error("Mix_PlayChannel: Error! %s\n",Mix_GetError());
    }
}
