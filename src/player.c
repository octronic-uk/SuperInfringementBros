#include "player.h"
#include <stdlib.h>

player_t* playerAllocate() {
    player_t* p = (player_t*)malloc(sizeof(player_t));
    p->pos.x  = 0;
    p->pos.y  = 0;
    p->health = 0;
    p->score  = 0;
    return p;
}
