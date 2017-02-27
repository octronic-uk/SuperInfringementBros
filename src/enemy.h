#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"

typedef struct enemy enemy_t;

struct enemy {
    char       state;
    sprite_t  *sprite;
    vector2i_t position;
    vector2f_t velocity;
    vector2f_t velocityDecay;
    int health;
    float projectileDelay;
    float lastProjectile;
    float spawnTime;
    // Path update function
    void (*updateFunction )(enemy_t* enemy, float currentTime, float deltaTime);
};

enemy_t* enemyAllocate(sprite_t* sprite);
void enemyDestroy(enemy_t* self);

#endif // ENEMY_H
