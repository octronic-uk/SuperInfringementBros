#include "vfx.h"


vfx_t *vfxAllocate(sprite_t* sprite) {
    vfx_t *self = (vfx_t*)malloc(sizeof(vfx_t));
    self->sprite = sprite;

    self->position.x = 0;
    self->position.y = 0;

    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;

    return self;
}

void vfxDestroy(vfx_t* self) {
    if (self != NULL) {
        if (self->sprite != NULL) {
            spriteDestroy(self->sprite);
            self->sprite = NULL;
        }
        free(self);
    }
}


