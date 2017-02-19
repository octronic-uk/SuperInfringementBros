#include "collectable.h"

collectable_t *collectableAllocate(sprite_t* sprite, char type) {
    printf("Allocating collectable_t\n"); 
    collectable_t *c = (collectable_t*)malloc(sizeof(collectable_t));
    c->sprite = sprite;
    c->position.x = 0;
    c->position.y = 0;
    c->velocity.x = 0.0f;
    c->velocity.y = 0.0f;
    c->type = type;
    return c;
}

void collectableDestroy(collectable_t* self) {
    printf("Destroying collectable_t\n"); 
    if (self != NULL) {
        if (self->sprite != NULL) {
            spriteDestroy(self->sprite);
            self->sprite = NULL;
        }
        free(self);
    }
}


