#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "vector.h"
#include "sprite.h"

typedef struct projectile projectile_t;

struct projectile{
    sprite_t* sprite;
    vector2i_t position;
    vector2f_t velocity;
    char type;
    int damage;
    int collisions;
    void (*updateFunction)(projectile_t* self, void* engine);
};

projectile_t* projectileAllocate(sprite_t* sprite);
void projectileDestroy(projectile_t* proj);

#endif // PROJECTILE_H
