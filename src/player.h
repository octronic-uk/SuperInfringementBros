#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"
#include "sprite.h"

#define DEFAULT_PLAYER_SPEED 0.5f

typedef struct {
    vector2i_t position;
    vector2f_t velocity;
    float speed;
    int score;
    int health;
    sprite_t* sprite;
} player_t;

player_t* playerAllocate(sprite_t*);
void playerDestroy(player_t*);

#endif // PLAYER_H

