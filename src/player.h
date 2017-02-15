#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"
#include "sprite.h"

typedef struct {
    vector2i_t position;
    vector2i_t velocity;
    int score;
    int health;
    sprite_t* sprite;
} player_t;

player_t* playerAllocate(sprite_t*);
void playerDestroy(player_t*);

#endif // PLAYER_H

