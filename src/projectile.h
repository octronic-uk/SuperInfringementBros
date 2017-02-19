#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "vector.h"
#include "sprite.h"

typedef struct {
    sprite_t* sprite;
    vector2i_t position;
    vector2f_t velocity;
    char type;
} projectile_t;

projectile_t* projectileAllocate(sprite_t* sprite);
void projectileDestroy(projectile_t* proj);

#endif // PROJECTILE_H
