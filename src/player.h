#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"
#include "sprite.h"

typedef struct {
    vector2i_t position;
    vector2f_t velocity;
    float speed;
    sprite_t* sprite;
    // Stats
    int health;
    int score;
    int numCoins;
    int numPunches;
    char state;
    // Projectlie Vars
    float lastProjectile;
    float projectileDelay;
    float lastPunch;
    float punchDelay;
} player_t;

player_t* playerAllocate(sprite_t*);
void playerDestroy(player_t*);

#endif // PLAYER_H

