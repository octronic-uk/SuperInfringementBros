#include "path.h"
#include <stdlib.h>

path_t *pathAllocate(int numFrames) {
    path_t *path = (path_t*)malloc(sizeof(path_t));
    path->frames = (path_frame_t**)malloc(sizeof(path_frame_t*)*numFrames);
    return path;
}
void pathDestroy(path_t *self) {
    if (self != NULL) {
        if (self->frames != NULL) {
            free(self->frames);
            self->frames = NULL;
        }
        free(self);
    }
}


