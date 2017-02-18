#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"
#include "path.h"

typedef struct {
    sprite_t  *sprite;
    vector2i_t position;
    vector2f_t velocity;
    vector2f_t velocityDecay;
    int health;
    path_t *path;
} enemy_t;

enemy_t* enemyAllocate(sprite_t* sprite, path_t* path);
void enemyDestroy(enemy_t* self);

#endif // ENEMY_H
