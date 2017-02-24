#include "music.h"
#include "logger.h"

music_t* musicAllocate(char* audioFile, int loops) {
    debug("Allocating music_t from %s loop=%d\n",audioFile,loops);
    music_t *music = (music_t*)malloc(sizeof(music_t));
    music->music = Mix_LoadMUS(audioFile);
    if (music->music == NULL) {
        printf("Error loading music_t %s\n",audioFile);
        free(music);
        return NULL;
    }
    music->loops = loops;
    return music;
}

void musicDestroy(music_t* self) {
    debug("Destroying music_t\n");
    if (self != NULL) {
        if(self->music != NULL) {
            Mix_FreeMusic(self->music);
            self->music = NULL;
        }
        free(self);
    }
}

void musicPlay(music_t* self) {
    if(Mix_PlayMusic(self->music, self->loops) == -1) {
       error("Mix_PlayMusic: Error! %s\n",Mix_GetError());
    }
}
