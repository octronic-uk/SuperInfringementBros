#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "vector.h"
#include "sprite.h"

#define DEFAULT_PROJECTILE_VELOCITY_X 1.0f
#define DEFAULT_PROJECTILE_VELOCITY_Y 0.0f
#define DEFAULT_PROJECTILE_DELAY     100.0f

#define PROJ_TYPE_PUNCH     1
#define PROJ_TYPE_BOOMERANG 2 
#define PROJ_TYPE_BULLET    3 

typedef struct {
    sprite_t* sprite;
    vector2i_t position;
    vector2f_t velocity;
    char type;
} projectile_t;

projectile_t* projectileAllocate(sprite_t* sprite);
void projectileDestroy(projectile_t* proj);

#endif // PROJECTILE_H
