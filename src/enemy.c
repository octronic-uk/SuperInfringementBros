#include "enemy.h"
#include <stdio.h>


enemy_t* enemyAllocate(sprite_t* sprite, path_t* path) {
    printf("Allocating enemy_t\n");
    enemy_t *enemy = (enemy_t*)malloc(sizeof(enemy_t));
    enemy->sprite = sprite;
    enemy->path = path;

    enemy->position.x = 0;
    enemy->position.y = 0;

    enemy->velocity.x = 0.0f;
    enemy->velocity.y = 0.0f;

    enemy->health = 0;

    return enemy;
}

void enemyDestroy(enemy_t* self) {
    printf("Destroying enemy_t\n");
    if (self != NULL) {
        if (self->sprite != NULL) {
            spriteDestroy(self->sprite);
            self->sprite = NULL;
        }
        if (self->path != NULL) {
            pathDestroy(self->path);
            self->path = NULL;
        }
    }
}


