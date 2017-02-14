#include "player.h"
#include <stdlib.h>
#include <stdio.h>

player_t* playerAllocate() {
    printf("Allocating new player_t\n"); 
    player_t* p = (player_t*)malloc(sizeof(player_t));
    p->pos.x  = 0;
    p->pos.y  = 0;
    p->health = 0;
    p->score  = 0;
    return p;
}


void playerDestroy(player_t* self) {
    printf("Destroying player_t\n"); 
    free(self);
}
