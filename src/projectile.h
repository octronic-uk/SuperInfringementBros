#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "vector.h"
#include "sprite.h"

#define DEFAULT_PROJECTILE_VELOCITY_X 0.5f
#define DEFAULT_PROJECTILE_VELOCITY_Y 0.0f
#define DEFAULT_PROJECTILE_DELAY     125.0f

typedef struct {
    sprite_t* sprite;
    vector2i_t position;
    vector2f_t velocity;
} projectile_t;

projectile_t* projectileAllocate(sprite_t* sprite);
void projectileDestroy(projectile_t* proj);

#endif // PROJECTILE_H
