#include <stdlib.h>
#include <stdio.h>

#include "player.h"
#include "constants.h"
#include "logger.h"

player_t* playerAllocate(sprite_t* sprite) {
    debug("Allocating new player_t\n"); 
    player_t* p = (player_t*)malloc(sizeof(player_t));
    p->velocity.x = 0;
    p->velocity.y = 0;
    p->position.x  = 0;
    p->position.y  = 0;
    p->health = 0;
    p->score  = 0;
    p->sprite = sprite;
    p->speed = PLAYER_DEFAULT_SPEED;
    p->numCoins = 0;
    p->numPunches = 5;


    p->projectileDelay = PROJECTILE_DEFAULT_DELAY;
    p->lastProjectile = 0.0f;

    p->punchDelay = PROJECTILE_DEFAULT_PUNCH_DELAY;
    p->lastPunch = 0.0f;


    return p;
}


void playerDestroy(player_t* self) {
    debug("Destroying player_t\n"); 
    if (self != NULL) {
        if (self->sprite != NULL) {
            spriteDestroy(self->sprite);
        }
        free(self);
    }
}
