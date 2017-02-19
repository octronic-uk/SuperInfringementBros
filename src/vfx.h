#ifndef VFX_H
#define VFX_H

#include "sprite.h"
#include "vector.h"

typedef struct {
    sprite_t   *sprite;
    vector2i_t position;
    vector2f_t velocity;
} vfx_t;

vfx_t *vfxAllocate(sprite_t* sprite);
void vfxDestroy(vfx_t* self);

#endif // VFX_H
