#ifndef PATH_H
#define PATH_H

#include <stdlib.h>

#include "vector.h"

typedef struct {
    vector2i_t position;
    float      duration; 
} path_frame_t;

typedef struct {
    path_frame_t **frames;
    int currentFrame;
    float currentTime;
    int numFrames;
} path_t;

path_t *pathAllocate();
void pathDestroy(path_t *self);

#endif  // PATH_H
