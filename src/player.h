#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"

typedef struct {
    vector2i_t pos;
    int score;
    int health;
} player_t;

player_t* playerAllocate();
void playerDestroy(player_t*);

#endif // PLAYER_H

