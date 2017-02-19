#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "vector.h"
#include "sprite.h"

typedef struct {
    char        type;
    sprite_t   *sprite;
    vector2i_t  position;
    vector2f_t  velocity;
} collectable_t;

collectable_t *collectableAllocate(sprite_t* sprite, char type);
void           collectableDestroy(collectable_t*);

#endif  // COLLECTABLE_H
