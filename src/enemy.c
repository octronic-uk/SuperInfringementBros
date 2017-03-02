#include <SDL2/SDL.h>

#include "enemy.h"
#include "constants.h"
#include "logger.h"

enemy_t* enemyAllocate(sprite_t* sprite) {
    debug("Allocating enemy_t\n");
    enemy_t *enemy = (enemy_t*)malloc(sizeof(enemy_t));
    enemy->sprite = sprite;

    enemy->position.x = 0;
    enemy->position.y = 0;

    enemy->velocity.x = 0.0f;
    enemy->velocity.y = 0.0f;

    enemy->velocityDecay.x = 0.0f;
    enemy->velocityDecay.y = 0.0f;

    enemy->health = 0;
    enemy->lastProjectile = 0.0f;
    enemy->projectileDelay = ENEMY_PROJECTILE_DELAY;
    enemy->updateFunction = NULL;
    enemy->spawnTime = SDL_GetTicks();
    enemy->fireProjectileFunction = NULL;

    return enemy;
}

void enemyDestroy(enemy_t* self) {
    debug("Destroying enemy_t\n");
    if (self != NULL) {
        if (self->sprite != NULL) {
            spriteDestroy(self->sprite);
            self->sprite = NULL;
        }
    }
}


