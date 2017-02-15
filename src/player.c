#include "player.h"
#include <stdlib.h>
#include <stdio.h>

player_t* playerAllocate(sprite_t* sprite) {
    printf("Allocating new player_t\n"); 
    player_t* p = (player_t*)malloc(sizeof(player_t));
    p->velocity.x = 0;
    p->velocity.y = 0;
    p->position.x  = 0;
    p->position.y  = 0;
    p->health = 0;
    p->score  = 0;
    p->sprite = sprite;
    return p;
}


void playerDestroy(player_t* self) {
    printf("Destroying player_t\n"); 
    if (self != NULL) {
        if (self->sprite != NULL) {
            spriteDestroy(self->sprite);
        }
        free(self);
    }
}
