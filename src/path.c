#include <stdlib.h>

#include "path.h"
#include "logger.h"

path_t *pathAllocate(int numFrames) {
    debug("Allocating path_t\n");
    path_t *path = (path_t*)malloc(sizeof(path_t));
    path->frames = (path_frame_t**)malloc(sizeof(path_frame_t*)*numFrames);
    return path;
}
void pathDestroy(path_t *self) {
    debug("Destroying path_t\n");
    if (self != NULL) {
        if (self->frames != NULL) {
            free(self->frames);
            self->frames = NULL;
        }
        free(self);
    }
}


