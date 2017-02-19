#include "engine.h"
#include "constants.h"
#include <stdio.h>

engine_t* engineAllocate() {
    printf("Allocating new engine_t\n");
    engine_t* engine = (engine_t*)malloc(sizeof(engine_t));
    engine->event    = (SDL_Event*)malloc(sizeof(SDL_Event));

    // Malloc Backgrounds Array
    engine->backgrounds = (background_t**)malloc(sizeof(background_t*)*ENGINE_MAX_BACKGROUNDS);
    for (int bgIndex = 0; bgIndex < ENGINE_MAX_BACKGROUNDS; bgIndex++) {
        engine->backgrounds[bgIndex] = NULL; 
    }

    // Malloc Projectiles Array
    engine->projectiles = (projectile_t**)malloc(sizeof(projectile_t*)*ENGINE_MAX_PROJECTILES);
    for (int pIndex = 0; pIndex < ENGINE_MAX_PROJECTILES; pIndex++) {
        engine->projectiles[pIndex] = NULL; 
    }

    // Malloc SFX Array
    engine->sfx = (sfx_t**)malloc(sizeof(sfx_t*)*ENGINE_MAX_SFX);
    for (int sIndex = 0; sIndex < ENGINE_MAX_SFX; sIndex++) {
        engine->sfx[sIndex] = NULL; 
    }

    // Malloc Enemy Array
    engine->enemies = (enemy_t**)malloc(sizeof(enemy_t*)*ENGINE_MAX_ENEMIES);
    for (int eIndex = 0; eIndex < ENGINE_MAX_ENEMIES; eIndex++) {
        engine->enemies[eIndex] = NULL; 
    }

    // Malloc vfx Array
    engine->vfx = (vfx_t**)malloc(sizeof(vfx_t*)*ENGINE_MAX_VFX);
    for (int vIndex = 0; vIndex < ENGINE_MAX_VFX; vIndex++) {
        engine->vfx[vIndex] = NULL; 
    }

    // Malloc collectables array
    engine->collectables = (collectable_t**)malloc(sizeof(collectable_t*)*ENGINE_MAX_COLLECTABLES);
    for (int cIndex=0; cIndex<ENGINE_MAX_COLLECTABLES; cIndex++) {
        engine->collectables[cIndex] = NULL;
    }

    // Init Variables
    engine->currentTime = 0.0f;
    engine->lastTime    = 0.0f;
    engine->deltaTime   = 0.0f;;

    engine->window   = NULL;
    engine->renderer = NULL;

    engine->inputFunction  = NULL;
    engine->updateFunction = NULL;
    engine->renderFunction = NULL;

    engine->upPressed    = 0;
    engine->downPressed  = 0;
    engine->leftPressed  = 0;
    engine->rightPressed = 0;
    engine->aBtnPressed  = 0;
    engine->bBtnPressed  = 0;

    engine->lastProjectile = 0.0f;
    engine->projectileDelay = PROJECTILE_DEFAULT_DELAY;

    return engine;
}

int engineInit(engine_t* self, int width, int height, char* title) {
    self->screenWidth = width;
    self->screenHeight = height;

    if (self == NULL) {
        printf("Engine: Error, engine_t must be allocated before initialisation.\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        printf("Engine: SDL_Init Error = %s\n" , SDL_GetError());
        return 1;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("Engine: failed to init SDL image %s\n",IMG_GetError());
        return 1;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        return 1;
    }

    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return 1;
    }

    self->window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_SHOWN);

    if (self->window == NULL) {
        printf("Engine: SDL_CreateWindow Error: %s\n",SDL_GetError());
        SDL_Quit();
        return 1;
    }

    self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (self->renderer == NULL) {
        SDL_DestroyWindow(self->window);
        printf("SDL_CreateRenderer Error: %s\n",SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (_setupResources(self) != ENGINE_OK) {
        printf("Error during resource setup\n");
        return ENGINE_ERROR;
    }

    return 0;
}

void engineDestroy(engine_t* self) {
    printf("Destroying engine_t\n");
    if (self != NULL) {
        // Delete SDL Objects
        if (self->renderer != NULL) {
            SDL_DestroyRenderer(self->renderer);
        }
        if (self->window != NULL) {
            SDL_DestroyWindow(self->window);
        }

        // Delete player
        if (self->player != NULL) {
            playerDestroy(self->player);
            self->player = NULL;
        }
        // Delete all backgrounds
        if (self->backgrounds != NULL) {
            for (int bgIndex = 0; bgIndex < ENGINE_MAX_BACKGROUNDS; bgIndex++) {
                if (self->backgrounds[bgIndex] != NULL) {
                    backgroundDestroy(self->backgrounds[bgIndex]);
                    self->backgrounds[bgIndex] = NULL;
                }
            }
            free(self->backgrounds);
            self->backgrounds = NULL;
        }
        // Delete all sfx
        if (self->sfx != NULL) {
            for (int sIndex=0;sIndex<ENGINE_MAX_SFX;sIndex++) {
                if (self->sfx[sIndex] != NULL) {
                    sfxDestroy(self->sfx[sIndex]);
                    self->sfx[sIndex] = NULL;
                }
            }
            free(self->sfx);
            self->sfx = NULL;
        }
        // Delete all projectiles
        if (self->projectiles != NULL) {
            for (int pIndex=0;pIndex<ENGINE_MAX_PROJECTILES;pIndex++) {
                if (self->projectiles[pIndex] != NULL) {
                    projectileDestroy(self->projectiles[pIndex]);
                    self->projectiles[pIndex] = NULL;
                }
            }
            free(self->projectiles);
            self->projectiles = NULL;
        }
        // Delete all enemies 
        if (self->enemies != NULL) {
            for (int eIndex=0; eIndex<ENGINE_MAX_ENEMIES; eIndex++) {
                if (self->enemies[eIndex] != NULL) {
                    enemyDestroy(self->enemies[eIndex]);
                    self->enemies[eIndex] = NULL;
                }
            }
            free(self->enemies);
            self->enemies = NULL;
        }
        // Delete all vfx
        if (self->vfx != NULL) {
            for (int vIndex=0; vIndex<ENGINE_MAX_VFX; vIndex++) {
                if (self->vfx[vIndex] != NULL) {
                    vfxDestroy(self->vfx[vIndex]);
                    self->vfx[vIndex] = NULL;
                }
            }
            free(self->vfx);
            self->vfx = NULL;
        }
        // Destroy all collectables
        if (self->collectables != NULL) {
            for (int cIndex=0; cIndex < ENGINE_MAX_COLLECTABLES; cIndex++) {
                if (self->collectables[cIndex] != NULL) {
                    collectableDestroy(self->collectables[cIndex]);
                    self->collectables[cIndex] = NULL;
                }
            } 
            free(self->collectables);
            self->collectables = NULL;
        }
        // Delete BGM Music
        if (self->bgm != NULL) {
            musicDestroy(self->bgm);
            self->bgm = NULL;
        }
        // Destroy Score Text
        if (self->score != NULL) {
            textDestroy(self->score);
            self->score = NULL;
        }
    }
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

int engineLoop(engine_t* self) {
    printf("Starting engine loop\n");
    char done = 0;
    do {
        self->currentTime = SDL_GetTicks();
        self->deltaTime = self->currentTime - self->lastTime;

        if (self->inputFunction != NULL) {
            int result = self->inputFunction(self);
            if (result == ENGINE_QUIT) {
                done = 1;
            }
        } else {
            done = 1;
        }

        if (self->updateFunction != NULL) {
            int result = self->updateFunction(self);
            if (result == ENGINE_QUIT) {
                done = 1;
            }
        } else {
            done = 1;
        }

        if (self->renderFunction != NULL) {
            int result = self->renderFunction(self);
            if (result == ENGINE_QUIT) {
                done = 1;
            }
        } else { 
            done = 1;
        }

        self->lastTime = self->currentTime;
    } while (done == 0);
    printf("Finished engine loop\n");
    return ENGINE_OK;
}

int engineDefaultInputHandler(engine_t* self) { 
    SDL_PollEvent(self->event);
    if (self->event->type == SDL_QUIT) {
        return ENGINE_QUIT;
    } else if (self->event->type == SDL_KEYDOWN) {
        switch (self->event->key.keysym.sym) {
            case SDLK_d:
                if (self->debug) {
                    printf("Debug Disabled\n");
                    self->debug = 0;
                } else {
                    printf("Debug Enabled\n");
                    self->debug = 1;
                }
                break;

            case SDLK_UP:
                self->upPressed = 1;
                break;
            case SDLK_DOWN:
                self->downPressed = 1;
                break;
            case SDLK_LEFT:
                self->leftPressed = 1;
                break;
            case SDLK_RIGHT:
                self->rightPressed = 1;
                break;
            case SDLK_a:
                self->aBtnPressed = 1;
                break;
            case SDLK_s:
                self->bBtnPressed = 1;
                break;
        }
    } else if (self->event->type == SDL_KEYUP) {
        // KeyUp
        switch (self->event->key.keysym.sym) {
            case SDLK_UP:
                self->upPressed = 0;
                break;
            case SDLK_DOWN:
                self->downPressed = 0;
                break;
            case SDLK_LEFT:
                self->leftPressed = 0;
                break;
            case SDLK_RIGHT:
                self->rightPressed = 0;
                break;
            case SDLK_a:
                self->aBtnPressed = 0;
                break;
            case SDLK_s:
                self->bBtnPressed = 0;
                break;
        }
    } 
    return ENGINE_OK;
}

int engineDefaultUpdateHandler(engine_t* self) { 
    if (self->debug)  {
        printf("DeltaTime: %.2fms\n",self->deltaTime);
    }
    _updateBackgrounds(self);
    _updatePlayer(self);
    _updateProjectiles(self);
    _updateCollectables(self);
    _updateEnemies(self);
    _updateVfx(self);
    // Done
    return ENGINE_OK;
}  

int engineGetProjectileIndex(engine_t *self, projectile_t* projectile) {
    for (int pIndex = 0; pIndex<ENGINE_MAX_PROJECTILES; pIndex++){
        if (self->projectiles[pIndex] == projectile) {
            return pIndex;
        }
    }
    return -1;
}

int engineDefaultRenderHandler(engine_t* self) {
    // Clear
    SDL_RenderClear(self->renderer);
    _renderBackgrounds(self);
    _renderPlayer(self);
    _renderEnemies(self);
    _renderProjectiles(self);
    _renderVfx(self);
    _renderCollectables(self);
    _renderScore(self);
    SDL_RenderPresent(self->renderer);
    return ENGINE_OK;
}  

// UPDATE ======================================================================

void _updateBackgrounds(engine_t* self) {
    // Scroll Backgrounds
    for (int bgIndex = 0; bgIndex < ENGINE_MAX_BACKGROUNDS; bgIndex++) {

        background_t *nextBg = self->backgrounds[bgIndex];

        if (nextBg == NULL) {
            continue;
        } 

        if (nextBg->scroll == 1) {
            nextBg->position.x += nextBg->velocity.x*self->deltaTime;
            nextBg->position.y += nextBg->velocity.y*self->deltaTime;
        }

        if (nextBg->repeatScroll == 1) {
            if (nextBg->position.x < -(nextBg->dimensions.x/2)) {
                nextBg->position.x = -1;
            }

            if (nextBg->position.y < -(nextBg->dimensions.y/2)) {
                nextBg->position.y = -1;
            }
        }
    }
}

void _updatePlayer(engine_t* self) {
    // Process Player Motion Input
    // Set Velocities
    // X Axis
    if (self->leftPressed == 1 && self->rightPressed == 1) {
        self->player->velocity.x = 0.0f;
    }
    else if (self->leftPressed == 1 ) { 
        self->player->velocity.x = -self->player->speed*self->deltaTime;
    }
    else if (self->rightPressed == 1) {
        self->player->velocity.x = self->player->speed*self->deltaTime;
    }
    else {
        self->player->velocity.x = 0.0f;
    }
    // Move X
    if (self->player->velocity.x < 0.0f)  { // Left
        if (self->player->position.x > 0) {
            self->player->position.x += self->player->velocity.x;
        } else {
            self->player->position.x = 0;
            self->player->velocity.x = 0.0f;
        }
    } 
    else if (self->player->velocity.x > 0.0f) { // Right
        if (self->player->position.x < self->screenWidth - self->player->sprite->dimensions.x) {
            self->player->position.x += self->player->velocity.x;
        }
        else {
            self->player->position.x = self->screenWidth - self->player->sprite->dimensions.x;
            self->player->velocity.x = 0.0f;
        }
    }

    // Y Axis
    if (self->upPressed == 1 && self->downPressed == 1) {
        self->player->velocity.y = 0.0f;
    }
    else if (self->upPressed == 1 ) { 
        self->player->velocity.y = -self->player->speed*self->deltaTime;
    } else if (self->downPressed == 1){
        self->player->velocity.y = self->player->speed*self->deltaTime;
    } else {
        self->player->velocity.y = 0.0f;
    }
    // Move Y
    if (self->player->velocity.y < 0.0f)  { // Up 
        if (self->player->position.y > 0) {
            self->player->position.y += self->player->velocity.y;
        } else {
            self->player->position.y = 0;
            self->player->velocity.y = 0.0f;
        }
    } else if (self->player->velocity.y > 0) { // Down
        if (self->player->position.y < self->screenHeight - self->player->sprite->dimensions.y) {
            self->player->position.y += self->player->velocity.y;
        }
        else {
            self->player->position.y = self->screenHeight - self->player->sprite->dimensions.y;
            self->player->velocity.y = 0.0f;
        }
    }

    if (self->aBtnPressed == 1) {
        _createBoomerangProjectile(self);
    }
    else if (self->bBtnPressed == 1) {
        _createPunchProjectile(self);
    }
}

void _updateVfx(engine_t* self) {
    for (int vIndex=0; vIndex<ENGINE_MAX_VFX; vIndex++) {
        vfx_t *nextVfx = self->vfx[vIndex];

        if (nextVfx == NULL) {
            continue;
        }

        nextVfx->position.x += nextVfx->velocity.x;
        nextVfx->position.y += nextVfx->velocity.y;


        if (spriteAnimationFinished(nextVfx->sprite)) {
            if (nextVfx->type == VFX_TYPE_ENEMY_EXPLOSION) {
                _createCoinCollectable(self,nextVfx->position);
            }
            vfxDestroy(nextVfx);
            self->vfx[vIndex] = NULL;
        }
    }
}

void _updateCollectables(engine_t* self) {
    for (int cIndex=0; cIndex<ENGINE_MAX_COLLECTABLES; cIndex++) {
        collectable_t *nextCollectable = self->collectables[cIndex];
        if (nextCollectable == NULL) {
            continue;
        }
        // Update position
        nextCollectable->position.x += nextCollectable->velocity.x;
        nextCollectable->position.y += nextCollectable->velocity.y;
        // Check for collision
    }
}

void _updateEnemies(engine_t* self) {
    // Update Enemies
    int activeEnemies = 0;
    for (int eIndex = 0; eIndex < ENGINE_MAX_ENEMIES; eIndex++) {
        enemy_t *nextEnemy = self->enemies[eIndex];
        // Not set
        if (nextEnemy == NULL) {
            continue;
        } 
        // Out of bounds
        if (nextEnemy->position.x < 0-nextEnemy->sprite->dimensions.x) {
            enemyDestroy(nextEnemy);
            self->enemies[eIndex] = NULL;
            continue;
        }
        // Dead
        if (nextEnemy->health < 0) {
            nextEnemy->state = ENEMY_STATE_DEAD;
            _explodeEnemy(self,nextEnemy);
            enemyDestroy(nextEnemy);
            self->enemies[eIndex] = NULL;
            continue;
        }

        // Needs Update
        nextEnemy->position.x += nextEnemy->velocity.x*self->deltaTime;
        nextEnemy->position.y += nextEnemy->velocity.y*self->deltaTime;

        if (self->debug == 1) {
            printf ("Enemy Velocity %f, Decay %f\n",nextEnemy->velocity.x,nextEnemy->velocityDecay.x);
        }

        if (nextEnemy->velocity.x > 0.1) {
            nextEnemy->velocity.x -= nextEnemy->velocityDecay.x;
        } else if (nextEnemy->velocity.x < -0.1) {
            nextEnemy->velocity.x += nextEnemy->velocityDecay.x;
        } else if (nextEnemy->velocity.x < 0.1 && nextEnemy->velocity.x > -0.1) {
            if (self->debug == 1) {
                printf("Reset velocity\n");
            }
            nextEnemy->state = ENEMY_STATE_PATH;
            nextEnemy->velocity.x = ENEMY_DEFAULT_VELOCITY;
            nextEnemy->velocityDecay.x = 0;
        }

        projectile_t** collisions = _getProjectileOnEnemyCollisions(self, nextEnemy); 

        for (int cIndex=0;cIndex<ENGINE_MAX_PROJECTILES;cIndex++) {
            projectile_t* collidedProjectile = collisions[cIndex];
            if (collidedProjectile == NULL) {
                continue;
            }

            int i = engineGetProjectileIndex(self, collidedProjectile);
            int whichSfx = -1;

            if (i >= 0) {
                if (collidedProjectile->type == PROJECTILE_TYPE_PUNCH) {
                    whichSfx = (rand()%2)+1;
                    nextEnemy->velocity.x = 2.0f; 
                    nextEnemy->velocityDecay.x = 0.1; 
                    nextEnemy->health -= 10;
                    nextEnemy->state = ENEMY_STATE_MOVE;
                } else if (collidedProjectile->type == PROJECTILE_TYPE_BOOMERANG) {
                    nextEnemy->health -= 25;
                    whichSfx = 3;
                }

                if (whichSfx >= 0) {
                    sfxPlay(self->sfx[whichSfx]);
                }

                self->player->score += 5;
                self->projectiles[i] = NULL;
                projectileDestroy(collidedProjectile); 
            }
        }
        free(collisions);

        // Fire a projectile if enough time has passed
        if (SDL_GetTicks() > nextEnemy->lastProjectile + nextEnemy->projectileDelay) {
            _createEnemyRocketProjectile(self,nextEnemy);
            nextEnemy->lastProjectile = SDL_GetTicks();
        }
        activeEnemies++;
    }
    if (self->debug) {
        printf("%d active enemies\n",activeEnemies);
    }
}

void _updateProjectiles(engine_t* self) {
    // Update Projectiles
    int activeProjectiles = 0;
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        projectile_t *nextProj = self->projectiles[projIndex];
        // Not set
        if (nextProj == NULL) {
            continue;
        } 
        // Out of bounds
        if (
                nextProj->position.x > self->screenWidth || 
                nextProj->position.x < 0-nextProj->sprite->tileDimensions.x
           ) {
            projectileDestroy(nextProj);
            self->projectiles[projIndex] = NULL;
            continue;
        }
        // Needs Update
        nextProj->position.x += nextProj->velocity.x*self->deltaTime;
        nextProj->position.y += nextProj->velocity.y*self->deltaTime;
        activeProjectiles++;
    }
    if (self->debug) {
        printf("%d active projectiles\n",activeProjectiles);
    }
}

// RENDER ======================================================================
//
void _renderBackgrounds(engine_t* self) {
    for (int bgIndex = 0; bgIndex < ENGINE_MAX_BACKGROUNDS; bgIndex++) {
        background_t *nextBg = self->backgrounds[bgIndex];
        if (nextBg == NULL) {
            continue;
        } 
        SDL_Rect dest;
        dest.w = nextBg->dimensions.x;
        dest.h = nextBg->dimensions.y;
        dest.x = nextBg->position.x;
        dest.y = nextBg->position.y;
        SDL_RenderCopy(self->renderer, nextBg->texture, NULL, &dest);
    }
}

void _renderPlayer(engine_t* self) {
    SDL_Rect playerDest;
    playerDest.w = self->player->sprite->dimensions.x;
    playerDest.h = self->player->sprite->dimensions.y;
    playerDest.x = self->player->position.x;
    playerDest.y = self->player->position.y;
    SDL_RenderCopy(self->renderer,self->player->sprite->texture,NULL,&playerDest);
}

void _renderEnemies(engine_t* self) {
    for (int eIndex=0; eIndex < ENGINE_MAX_ENEMIES; eIndex++) {
        enemy_t *nextEnemy = self->enemies[eIndex];
        if (nextEnemy == NULL) {
            continue;
        } 
        SDL_Rect *src, dest;
        src = spriteGetCurrentFrameRect(nextEnemy->sprite);
        dest.w = nextEnemy->sprite->tileDimensions.x;
        dest.h = nextEnemy->sprite->tileDimensions.y;
        dest.x = nextEnemy->position.x;
        dest.y = nextEnemy->position.y;
        SDL_RenderCopy(self->renderer, nextEnemy->sprite->texture, src, &dest);
        spriteAdvanceFrame(nextEnemy->sprite,self->deltaTime);
        if (src != NULL) {
            free (src);
        }
    }
}

void _renderVfx(engine_t* self) {
    for (int vIndex=0; vIndex < ENGINE_MAX_VFX; vIndex++) {
        vfx_t *nextVfx = self->vfx[vIndex];
        if (nextVfx == NULL) {
            continue;
        } 
        SDL_Rect *src, dest;
        src = spriteGetCurrentFrameRect(nextVfx->sprite);
        dest.w = nextVfx->sprite->tileDimensions.x;
        dest.h = nextVfx->sprite->tileDimensions.y;
        dest.x = nextVfx->position.x;
        dest.y = nextVfx->position.y;
        SDL_RenderCopy(self->renderer, nextVfx->sprite->texture, src, &dest);
        spriteAdvanceFrame(nextVfx->sprite,self->deltaTime);
        if (src != NULL) {
            free (src);
        }
    }
}

void _renderCollectables(engine_t* self) {
    for (int cIndex=0; cIndex < ENGINE_MAX_COLLECTABLES; cIndex++) {
        collectable_t *nextColl = self->collectables[cIndex];
        if (nextColl == NULL) {
            continue;
        } 
        SDL_Rect *src, dest;
        src = spriteGetCurrentFrameRect(nextColl->sprite);
        dest.w = nextColl->sprite->tileDimensions.x;
        dest.h = nextColl->sprite->tileDimensions.y;
        dest.x = nextColl->position.x;
        dest.y = nextColl->position.y;
        SDL_RenderCopy(self->renderer, nextColl->sprite->texture, src, &dest);
        spriteAdvanceFrame(nextColl->sprite,self->deltaTime);
        if (src != NULL) {
            free (src);
        }
    }
}

void _renderProjectiles(engine_t* self) {
    for (int projIndex=0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        projectile_t *nextProj = self->projectiles[projIndex];
        if (nextProj == NULL) {
            continue;
        } 
        SDL_Rect *src, dest;
        src = spriteGetCurrentFrameRect(nextProj->sprite);
        dest.w = nextProj->sprite->tileDimensions.x;
        dest.h = nextProj->sprite->tileDimensions.y;
        dest.x = nextProj->position.x;
        dest.y = nextProj->position.y;
        SDL_RenderCopy(self->renderer, nextProj->sprite->texture, src, &dest);
        spriteAdvanceFrame(nextProj->sprite,self->deltaTime);
        if (src != NULL) {
            free (src);
        }
    }
}

void _renderScore(engine_t* self) {
    if (self->score != NULL) {
        snprintf(self->score->text,self->score->bufferSize, "Score: %.6d",self->player->score);
        SDL_Surface *surface = TTF_RenderText_Solid(self->score->font,self->score->text,self->score->colour);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            printf("Error: Could not create score texture\n");
            return;
        }
        int iW, iH;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest;
        dest.x = 5;
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

// MISC ========================================================================

void _explodeEnemy(engine_t* self, enemy_t* enemy) {
    for (int vIndex = 0; vIndex < ENGINE_MAX_VFX; vIndex++) {
        if (self->vfx[vIndex] == NULL) {
            sprite_t* expSprite = _createExplosionSprite(self);
            vfx_t *explosion = vfxAllocate(expSprite,VFX_TYPE_ENEMY_EXPLOSION);

            explosion->position.x = enemy->position.x;
            explosion->position.y = enemy->position.y;

            explosion->velocity.x = enemy->velocity.x;
            explosion->velocity.y = enemy->velocity.y;

            self->vfx[vIndex] = explosion;

            int whichSfx = rand()%2;
            switch (whichSfx) {
                case 0:
                    sfxPlay(self->sfx[SFX_EXPLOSION_1]);
                    break;
                case 1:
                    sfxPlay(self->sfx[SFX_EXPLOSION_2]);
                    break;
            }
            break;
        }
    }
}

projectile_t** _getProjectileOnEnemyCollisions(engine_t* engine, enemy_t* enemy) {
    projectile_t** retval = (projectile_t**)malloc(sizeof(projectile_t*)*ENGINE_MAX_PROJECTILES);
    for (int pIndex = 0; pIndex < ENGINE_MAX_PROJECTILES; pIndex++) {
        retval[pIndex] = NULL; 
    }

    int collisionIndex = 0;
    for (int pIndex = 0; pIndex < ENGINE_MAX_PROJECTILES; pIndex++) {
        projectile_t* proj = engine->projectiles[pIndex]; 
        if (proj == NULL) {
            continue;
        }
        vector2i_t pPosition = proj->position;
        vector2i_t pDimensions = proj->sprite->tileDimensions;
        vector2i_t ePosition = enemy->position;
        vector2i_t eDimensions = enemy->sprite->tileDimensions;

        if (vector2iCollision(pPosition, pDimensions, ePosition, eDimensions) == 1) {
            retval[collisionIndex] = proj;
            collisionIndex++;
        } 
    }
    return retval;
}

int _setupResources(engine_t* engine) {
    // Setup Player
    sprite_t* playerSprite = spriteAllocate("res/gfx/ship_128.png",engine->renderer);
    engine->player = playerAllocate(playerSprite);
    engine->player->position.x = playerSprite->dimensions.x;
    engine->player->position.y = engine->screenHeight/2-playerSprite->dimensions.y/2;
    // Setup Backgrounds
    background_t *bg = backgroundAllocate("res/gfx/sky.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 0;
        bg->repeatScroll = 0;
        engine->backgrounds[0] = bg;
    }
    bg = backgroundAllocate("res/gfx/clouds.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -0.1f;
        engine->backgrounds[1] = bg;
    }
    bg = backgroundAllocate("res/gfx/clouds_2.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -0.2f;
        engine->backgrounds[2] = bg;
    }
    bg = backgroundAllocate("res/gfx/ground.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -1.0f;
        engine->backgrounds[3] = bg;
    }
    // Allocate Fireball SFX
    engine->sfx[SFX_SHOOT      ] = sfxAllocate("res/sfx/shoot.wav",       0);
    engine->sfx[SFX_PUNCH_1    ] = sfxAllocate("res/sfx/punch_1.wav",     0);
    engine->sfx[SFX_PUNCH_2    ] = sfxAllocate("res/sfx/punch_2.wav",     0);
    engine->sfx[SFX_BOING      ] = sfxAllocate("res/sfx/boing.wav",       0);
    engine->sfx[SFX_EXPLOSION_1] = sfxAllocate("res/sfx/explosion_1.wav", 0);
    engine->sfx[SFX_EXPLOSION_2] = sfxAllocate("res/sfx/explosion_2.wav", 0);
    // Setup Enemies
    int numEnemies = 10;
    for (int i=0; i< numEnemies; i++) {
        engine->enemies[i] = _createEnemy(engine,numEnemies,i);    
    }
    // Load BGM
    engine->bgm = musicAllocate("res/bgm/japanism.wav",-1);
    musicPlay(engine->bgm);
    // Setup Score Font
    engine->score = textAllocate("res/fonts/blocked.ttf",32,50);
    // Done
    return ENGINE_OK;
}

// CREATE ======================================================================

enemy_t* _createEnemy(engine_t* engine, int numEnemies, int i) {
    sprite_t* enemySprite = spriteAllocate("res/gfx/enemy.png",engine->renderer);
    enemy_t* enemy = enemyAllocate(enemySprite,NULL);
    enemy->position.x = engine->screenWidth - enemy->sprite->dimensions.x*2;
    enemy->position.y = (engine->screenHeight/(numEnemies+1)) * (i+1) - enemy->sprite->dimensions.y/2;

    enemy->velocity.x = -0.1; 
    enemy->velocity.y = 0.0f;
    enemy->health = 100;
    enemy->state = ENEMY_STATE_PATH;
    return enemy;
}

sprite_t* _createExplosionSprite(engine_t* engine) {
    sprite_t* explosion = spriteAllocateSpriteSheet("res/gfx/explosion.png",66,64,13,10,engine->renderer);
    explosion->repeat = 0;
    // 0 
    explosion->frameOrder[0].x = 0;
    explosion->frameOrder[0].y = 0;
    // 1
    explosion->frameOrder[1].x = 1;
    explosion->frameOrder[1].y = 0;
    // 2
    explosion->frameOrder[2].x = 2;
    explosion->frameOrder[2].y = 0;
    // 3 
    explosion->frameOrder[3].x = 3;
    explosion->frameOrder[3].y = 0;
    // 4
    explosion->frameOrder[4].x = 4;
    explosion->frameOrder[4].y = 0;
    // 5
    explosion->frameOrder[5].x = 5;
    explosion->frameOrder[5].y = 0;
    // 6
    explosion->frameOrder[6].x = 6;
    explosion->frameOrder[6].y = 0;
    // 7
    explosion->frameOrder[7].x = 7;
    explosion->frameOrder[7].y = 0;
    // 8
    explosion->frameOrder[8].x = 8;
    explosion->frameOrder[8].y = 0;
    // 9
    explosion->frameOrder[9].x = 9;
    explosion->frameOrder[9].y = 0;
    // 10
    explosion->frameOrder[10].x = 10;
    explosion->frameOrder[10].y = 0;
    // 11
    explosion->frameOrder[11].x = 11;
    explosion->frameOrder[11].y = 0;
    // 12
    explosion->frameOrder[12].x = 12;
    explosion->frameOrder[12].y = 0;

    return explosion;
}

sprite_t* _createBoomerangSprite(engine_t* engine) {
    sprite_t* boomerangSprite = spriteAllocateSpriteSheet("res/gfx/boomerang.png",32,32,8,25,engine->renderer);
    // 0 
    boomerangSprite->frameOrder[7].x = 0;
    boomerangSprite->frameOrder[7].y = 0;
    // 1
    boomerangSprite->frameOrder[6].x = 1;
    boomerangSprite->frameOrder[6].y = 0;
    // 2
    boomerangSprite->frameOrder[5].x = 2;
    boomerangSprite->frameOrder[5].y = 0;
    // 3
    boomerangSprite->frameOrder[4].x = 3;
    boomerangSprite->frameOrder[4].y = 0;
    // 4
    boomerangSprite->frameOrder[3].x = 4;
    boomerangSprite->frameOrder[3].y = 0;
    // 5 
    boomerangSprite->frameOrder[2].x = 5;
    boomerangSprite->frameOrder[2].y = 0;
    // 6 
    boomerangSprite->frameOrder[1].x = 6;
    boomerangSprite->frameOrder[1].y = 0;
    // 7 
    boomerangSprite->frameOrder[0].x = 7;
    boomerangSprite->frameOrder[0].y = 0;

    return boomerangSprite;
}

sprite_t* _createEnemyRocketSprite(engine_t* engine) {
    sprite_t* rocketSprite = spriteAllocateSpriteSheet("res/gfx/rocket.png",64,23,2,150,engine->renderer);
    // 0
    rocketSprite->frameOrder[0].x = 0;
    rocketSprite->frameOrder[0].y = 0;
    // 1
    rocketSprite->frameOrder[1].x = 1;
    rocketSprite->frameOrder[1].y = 0;

    return rocketSprite;
}

sprite_t* _createCoinSprite(engine_t* self) {
    sprite_t* coinSprite = spriteAllocateSpriteSheet("res/gfx/coin.png",30,30,6,50,self->renderer);
    // 0 
    coinSprite->frameOrder[0].x = 0;
    coinSprite->frameOrder[0].y = 0;
    // 1
    coinSprite->frameOrder[1].x = 1;
    coinSprite->frameOrder[1].y = 0;
    // 2
    coinSprite->frameOrder[2].x = 2;
    coinSprite->frameOrder[2].y = 0;
    // 3
    coinSprite->frameOrder[3].x = 3;
    coinSprite->frameOrder[3].y = 0;
    // 4
    coinSprite->frameOrder[4].x = 4;
    coinSprite->frameOrder[4].y = 0;
    // 5 
    coinSprite->frameOrder[5].x = 5;
    coinSprite->frameOrder[5].y = 0;

    return coinSprite;
}

void _createCoinCollectable(engine_t *self, vector2i_t pos) {
    for (int cIndex = 0; cIndex < ENGINE_MAX_COLLECTABLES; cIndex++) {
        if (self->collectables[cIndex] == NULL) {

            sprite_t* coinSprite = _createCoinSprite(self);
            collectable_t* coin = collectableAllocate(coinSprite, COLLECTABLE_TYPE_COIN);

            coin->position.x = pos.x;
            coin->position.y = pos.y;

            self->collectables[cIndex] = coin;

            break;
        }
    }
}

void _createBoomerangProjectile(engine_t* self) {
    // Has enough time passed
    if (SDL_GetTicks() < self->lastProjectile + self->projectileDelay) {
        return;
    }
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            
            sprite_t* boomerangSprite = _createBoomerangSprite(self);
            projectile_t *boomerang = projectileAllocate(boomerangSprite);

            boomerang->position.x = self->player->position.x+self->player->sprite->dimensions.x+8;
            boomerang->position.y = 
                self->player->position.y + (self->player->sprite->dimensions.y/2) -
                boomerangSprite->tileDimensions.y/2;

            boomerang->velocity.x = PROJECTILE_DEFAULT_VELOCITY_X;
            boomerang->velocity.y = PROJECTILE_DEFAULT_VELOCITY_Y;

            self->projectiles[projIndex] = boomerang;
            self->lastProjectile = SDL_GetTicks();

            boomerang->type = PROJECTILE_TYPE_BOOMERANG; 
            sfxPlay(self->sfx[0]);

            break;
        }
    }
}

void _createEnemyRocketProjectile(engine_t* self, enemy_t* enemy) {
    // Fire a projectile if enough time has passed
    if (SDL_GetTicks() < enemy->lastProjectile + enemy->projectileDelay) {
        return;
    }
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* rocketSprite = _createEnemyRocketSprite(self);
            projectile_t *rocket = projectileAllocate(rocketSprite);
            rocket->position.x = enemy->position.x-enemy->sprite->dimensions.x-8;
            rocket->position.y = 
                enemy->position.y + (enemy->sprite->dimensions.y/2) -
                rocketSprite->tileDimensions.y/2;
            rocket->velocity.x = -PROJECTILE_DEFAULT_VELOCITY_X/2;
            rocket->velocity.y = -PROJECTILE_DEFAULT_VELOCITY_Y;
            self->projectiles[projIndex] = rocket;
            rocket->type = PROJECTILE_TYPE_ENEMY_ROCKET; 
            enemy->lastProjectile = SDL_GetTicks();
            break;
        }
    }
}

void _createPunchProjectile(engine_t* self) {
    // Has enough time passed
    if (SDL_GetTicks() < self->lastProjectile + self->projectileDelay) {
        return;
    }
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* fistSprite = spriteAllocate("res/gfx/fist.png",self->renderer);
            projectile_t *fist = projectileAllocate(fistSprite);
            fist->position.x = self->player->position.x+self->player->sprite->dimensions.x+8;
            fist->position.y = 
                self->player->position.y + (self->player->sprite->dimensions.y/2) -
                fistSprite->tileDimensions.y/2;
            fist->velocity.x = PROJECTILE_DEFAULT_VELOCITY_X;
            fist->velocity.y = PROJECTILE_DEFAULT_VELOCITY_Y;
            self->projectiles[projIndex] = fist;
            self->lastProjectile = SDL_GetTicks();
            fist->type = PROJECTILE_TYPE_PUNCH; 
            sfxPlay(self->sfx[0]);
            break;
        }
    }
}

