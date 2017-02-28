#include "projectile.h"
#include "logger.h"

projectile_t* projectileAllocate(sprite_t* sprite) {
    debug("Allocating projectile_t\n");
    projectile_t *self = (projectile_t*)malloc(sizeof(projectile_t));
    self->sprite = sprite;
    self->position.x = 0;
    self->position.y = 0;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->type = 0;
    self->damage = 0;
    self->updateFunction = NULL;
    return self;
}

void projectileDestroy(projectile_t* self) {
    debug("Destroying projectile_t\n");
    if (self != NULL) {
        if (self->sprite != NULL) {
            spriteDestroy(self->sprite);
            self->sprite = NULL;
        }
        free(self);
        self = NULL;
    }
}
