#ifndef VFX_H
#define VFX_H

#include "sprite.h"
#include "vector.h"

typedef struct vfx vfx_t;

struct vfx {
    char        type;
    sprite_t   *sprite;
    vector2i_t  position;
    vector2f_t  velocity;
    // Path update function
    void (*updateFunction )(vfx_t* self, float currentTime, float deltaTime);
};

vfx_t *vfxAllocate(sprite_t* sprite, char type);
void vfxDestroy(vfx_t* self);

#endif // VFX_H
