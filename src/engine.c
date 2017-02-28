#include <stdio.h>
#include "engine.h"
#include "constants.h"
#include "logger.h"

engine_t* engineAllocate() {
    debug("Allocating new engine_t\n");
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

    // Malloc tfx Array
    engine->tfx = (text_t**)malloc(sizeof(text_t*)*ENGINE_MAX_TFX);
    for (int tIndex = 0; tIndex < ENGINE_MAX_TFX; tIndex++) {
        engine->tfx[tIndex] = NULL;
    }

    // Malloc collectables array
    engine->collectables = (collectable_t**)malloc(sizeof(collectable_t*)*ENGINE_MAX_COLLECTABLES);
    for (int cIndex=0; cIndex<ENGINE_MAX_COLLECTABLES; cIndex++) {
        engine->collectables[cIndex] = NULL;
    }
    // Screen Attributes
    engine->screenTitle  = NULL;
    engine->screenWidth  = -1;
    engine->screenHeight = -1;

    // HUD Elements
    engine->scoreText    = NULL;
    engine->coinSprite   = NULL;
    engine->coinsText    = NULL;
    engine->punchSprite  = NULL;
    engine->punchText    = NULL;
    engine->healthSprite = NULL;
    engine->healthText   = NULL;

    // Init Variables
    engine->currentTime     = 0.0f;
    engine->lastTime        = 0.0f;
    engine->deltaTime       = 0.0f;;
    engine->lastEnemySpawn  = 0.0f;
    engine->enemySpawnDelay = ENEMY_SPAWN_DELAY;

    engine->window         = NULL;
    engine->renderer       = NULL;
    engine->inputFunction  = NULL;
    engine->updateFunction = NULL;
    engine->renderFunction = NULL;

    // Event variables
    engine->upPressed        = 0;
    engine->downPressed      = 0;
    engine->leftPressed      = 0;
    engine->rightPressed     = 0;
    engine->fire1BtnPressed  = 0;
    engine->fire2BtnPressed  = 0;
    engine->buyBtnPressed    = 0;
    engine->weaponBtnPressed = 0;

    return engine;
}

int engineInit(engine_t* self, int width, int height, char* title) {
    if (self == NULL) {
        error("Engine: Error, engine_t must be allocated before initialisation.\n");
        return ENGINE_ERROR;
    }

    self->screenWidth  = width;
    self->screenHeight = height;
    self->screenTitle  = title;

    if (engineInitSDL(self) != ENGINE_OK) {
        error("Error during SDL init\n");
        return ENGINE_ERROR;
    }

    if (_setupResources(self) != ENGINE_OK) {
        error("Error during resource setup\n");
        return ENGINE_ERROR;
    }

    return ENGINE_OK;
}

int engineInitSDL(engine_t* self) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
        error("Engine: SDL_Init Error = %s\n" , SDL_GetError());
        return ENGINE_ERROR;
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        error("Engine: failed to init SDL image %s\n",IMG_GetError());
        return ENGINE_ERROR;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        error("Mix_OpenAudio: %s\n", Mix_GetError());
        return ENGINE_ERROR;
    }

    if(TTF_Init()==-1) {
        error("TTF_Init: %s\n", TTF_GetError());
        return ENGINE_ERROR;
    }

    self->window = SDL_CreateWindow(self->screenTitle, 0, 0, self->screenWidth, self->screenHeight, SDL_WINDOW_SHOWN);

    if (self->window == NULL) {
        error("Engine: SDL_CreateWindow Error: %s\n",SDL_GetError());
        SDL_Quit();
        return ENGINE_ERROR;
    }

    self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (self->renderer == NULL) {
        SDL_DestroyWindow(self->window);
        error("SDL_CreateRenderer Error: %s\n",SDL_GetError());
        SDL_Quit();
        return ENGINE_ERROR;
    }
    return ENGINE_OK;
}

void engineDestroySDL(engine_t* self) {
    // Delete SDL Objects
    if (self->renderer != NULL) {
        SDL_DestroyRenderer(self->renderer);
    }
    if (self->window != NULL) {
        SDL_DestroyWindow(self->window);
    }
}

void engineDestroyResources(engine_t* self) {
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
    // Delete all tfx
    if (self->tfx != NULL) {
        for (int tIndex=0; tIndex<ENGINE_MAX_TFX; tIndex++) {
            if (self->tfx[tIndex] != NULL) {
                textDestroy(self->tfx[tIndex]);
                self->tfx[tIndex] = NULL;
            }
        }
        free(self->tfx);
        self->tfx = NULL;
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
    if (self->scoreText != NULL) {
        textDestroy(self->scoreText);
        self->scoreText = NULL;
    }

    // Destroy Health Text
    if (self->healthText != NULL) {
        textDestroy(self->healthText);
        self->healthText = NULL;
    }
    // Destroy Health Sprite
    if (self->healthSprite != NULL) {
        spriteDestroy(self->healthSprite);
        self->healthSprite = NULL;
    }

    // Destroy Coin Text
    if (self->coinsText != NULL) {
        textDestroy(self->coinsText);
        self->coinsText = NULL;
    }
    // Destroy Coin Sprite
    if (self->coinSprite != NULL) {
        spriteDestroy(self->coinSprite);
        self->coinSprite = NULL;
    }

    // Destroy Punch Text
    if (self->punchText != NULL) {
        textDestroy(self->punchText);
        self->punchText = NULL;
    }
    // Destroy Punch Sprite
    if (self->punchSprite != NULL) {
        spriteDestroy(self->punchSprite);
        self->punchSprite = NULL;
    }
}

void engineDestroy(engine_t* self) {
    debug("Destroying engine_t\n");
    if (self != NULL) {
        engineDestroySDL(self);
        engineDestroyResources(self);
        engineCloseSDL(self);
        free(self);
    }
}

void engineCloseSDL(engine_t *self) {
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

int engineLoop(engine_t* self) {
    debug("Starting engine loop\n");
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
    debug("Finished engine loop\n");
    return ENGINE_OK;
}

int engineDefaultInputHandler(engine_t* self) {
    SDL_PollEvent(self->event);
    if (self->event->type == SDL_QUIT) {
        return ENGINE_QUIT;
    } else if (self->event->type == SDL_KEYDOWN) {
        switch (self->event->key.keysym.sym) {
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
                self->fire1BtnPressed = 1;
                break;
            case SDLK_s:
                self->fire2BtnPressed = 1;
                break;
            case SDLK_d:
                self->weaponBtnPressed = 1;
                break;
            case SDLK_f:
                self->buyBtnPressed = 1;
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
                self->fire1BtnPressed = 0;
                break;
            case SDLK_s:
                self->fire2BtnPressed = 0;
                break;
            case SDLK_d:
                self->weaponBtnPressed = 0;
                break;
            case SDLK_f:
                self->buyBtnPressed = 0;
                break;
        }
    }
    return ENGINE_OK;
}

int engineDefaultUpdateHandler(engine_t* self) {
    info("DeltaTime: %.2fms\n",self->deltaTime);
    _updateBackgrounds(self);
    _updatePlayer(self);
    _updateProjectiles(self);
    _updateCollectables(self);
    _updateEnemies(self);
    _updateVfx(self);
    _updateTfx(self);
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
    _renderTfx(self);
    _renderHUD(self);
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
    if (self->player->state == PLAYER_STATE_DEAD) {
        return;
    }
    // Player exists
    if (self->player != NULL) {
        // U dead?
        if (self->player->health <= 0) {
            _explodePlayer(self);     
            self->player->state = PLAYER_STATE_DEAD;
            return;
        }
        // Check projectile collisions
        vector2i_t playerPosition = self->player->position;
        vector2i_t playerDimensions = self->player->sprite->tileDimensions;

        for (int pIndex=0; pIndex<ENGINE_MAX_PROJECTILES; pIndex++) {
            projectile_t* proj = self->projectiles[pIndex];
            if (proj==NULL) {
                continue;
            }
            vector2i_t projPosition = proj->position;
            vector2i_t projDimensions = proj->sprite->tileDimensions;
            int hasCollided = vector2iScaledCollision(playerPosition,playerDimensions,projPosition,projDimensions,0.75,0.75);
            if(hasCollided) {
                self->player->health -= proj->damage;
                projectileDestroy(proj);
                self->projectiles[pIndex] = NULL;
            }
        }
        // Check enemy collisions
        for (int eIndex=0; eIndex<ENGINE_MAX_ENEMIES; eIndex++) {
            enemy_t* enemy = self->enemies[eIndex];
            if (enemy == NULL) {
                continue;
            }
            vector2i_t ePosition = enemy->position;
            vector2i_t eDimensions = enemy->sprite->tileDimensions;
            int hasCollided = vector2iScaledCollision(playerPosition,playerDimensions,ePosition,eDimensions,0.75,0.75);
            if(hasCollided) {
                self->player->health = 0;
                _explodeEnemy(self,enemy);
                enemyDestroy(enemy);
                self->enemies[eIndex] = NULL;
                self->projectiles[eIndex] = NULL;
                return;
            }
        }

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

        if (self->fire1BtnPressed == 1) {
            _fireDefaultProjectile(self);
        }
        else if (self->fire2BtnPressed == 1) {
            if (self->player->numCoins >= PROJECTILE_PUNCH_COST) {
                _insertPunchProjectile(self);
            }
        }
    }
}

void _updateTfx(engine_t* self) {
    for (int tIndex=0; tIndex<ENGINE_MAX_TFX; tIndex++) {
        text_t *nextTfx = self->tfx[tIndex];

        if (nextTfx == NULL) {
            continue;
        }

        if (nextTfx->updateFunction != NULL) {
            nextTfx->updateFunction(nextTfx,(void*)self);
        }

        if (nextTfx->position.y < -100 || nextTfx->colour.a == 0) {
            textDestroy(nextTfx);
            self->tfx[tIndex] = NULL;
        }
    }
}

void _updateVfx(engine_t* self) {
    for (int vIndex=0; vIndex<ENGINE_MAX_VFX; vIndex++) {
        vfx_t *nextVfx = self->vfx[vIndex];

        if (nextVfx == NULL) {
            continue;
        }

        if (nextVfx->updateFunction != NULL) {
            nextVfx->updateFunction(nextVfx,(void*)self);
        }

        if (spriteAnimationFinished(nextVfx->sprite)) {
            if (nextVfx->type == VFX_TYPE_ENEMY_EXPLOSION) {
                vector2i_t pos;
                pos.x = nextVfx->position.x + nextVfx->sprite->tileDimensions.x/2;
                pos.y = nextVfx->position.y + nextVfx->sprite->tileDimensions.y/2;
                _insertCoinCollectable(self,pos);
            }
            vfxDestroy(nextVfx);
            self->vfx[vIndex] = NULL;
        }
    }
}

void _updateCollectables(engine_t* self) {
    vector2i_t pPosition   = self->player->position;
    vector2i_t pDimensions = self->player->sprite->tileDimensions;

    for (int cIndex=0; cIndex<ENGINE_MAX_COLLECTABLES; cIndex++) {
        collectable_t *nextCol = self->collectables[cIndex];
        if (nextCol == NULL) {
            continue;
        }

        vector2i_t cPosition   = nextCol->position;
        vector2i_t cDimensions = nextCol->sprite->tileDimensions;

        switch (nextCol->type) {
            case COLLECTABLE_TYPE_COIN:
                if (vector2iCollision(cPosition, cDimensions, pPosition, pDimensions) == 1) {
                    collectableDestroy(nextCol);
                    self->collectables[cIndex] = NULL;
                    sfxPlay(self->sfx[SFX_COIN_GET]);
                    self->player->numCoins++;
                    self->player->score += 100;
                    continue;
                }
                break;
        }

        // Update position
        nextCol->position.x += nextCol->velocity.x;
        nextCol->position.y += nextCol->velocity.y;
    }
}

void _vfxLinearPath(vfx_t* self, void* vEngine) {
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
}

void _enemyLinearPath(enemy_t* enemy, void* vEngine) {
    enemy->position.x += enemy->velocity.x * ((engine_t*)vEngine)->deltaTime;
}

void _spawnEnemy(engine_t* self) {
    if (self->currentTime > self->lastEnemySpawn + self->enemySpawnDelay) {
        for (int eIndex=0; eIndex<ENGINE_MAX_ENEMIES; eIndex++) {
            if (self->enemies[eIndex] == NULL) {

                sprite_t* enemySprite = spriteAllocate("res/gfx/enemy.png",self->renderer);
                enemy_t* enemy = enemyAllocate(enemySprite);

                int spriteHeight = enemy->sprite->dimensions.y;
                int spriteWidth = enemy->sprite->dimensions.x;

                enemy->position.x = self->screenWidth;
                enemy->position.y = ((rand() % self->screenHeight) - (spriteHeight*2)) + spriteHeight;

                enemy->velocity.x = -0.1;
                enemy->velocity.y = 0.0f;
                enemy->health = 100;
                enemy->state = ENEMY_STATE_PATH;
                enemy->updateFunction = &_enemyLinearPath;
                self->enemies[eIndex] = enemy;
                self->lastEnemySpawn = self->currentTime;
                break;
            }
        }
    }
}

void _updateEnemies(engine_t* self) {
    _spawnEnemy(self);
    // Update Enemies
    int activeEnemies = 0;
    for (int eIndex = 0; eIndex < ENGINE_MAX_ENEMIES; eIndex++) {
        enemy_t *nextEnemy = self->enemies[eIndex];
        // Not set
        if (nextEnemy == NULL) {
            continue;
        }
        // Out of bounds
        if (nextEnemy->position.x < 0-nextEnemy->sprite->dimensions.x*2) {
            enemyDestroy(nextEnemy);
            self->enemies[eIndex] = NULL;
            continue;
        }
        // Dead
        if (nextEnemy->health <= 0) {
            _insertPopupText(self,"1000",nextEnemy->position);
            self->player->score += 1000;
            _explodeEnemy(self,nextEnemy);
            enemyDestroy(nextEnemy);
            self->enemies[eIndex] = NULL;
            continue;
        }

        // Needs Update
        switch(nextEnemy->state) {
            case ENEMY_STATE_PATH:
                nextEnemy->updateFunction(nextEnemy, (void*)self);
                break;
            case  ENEMY_STATE_PUNCHED:
                debug("Enemy Velocity %f, Decay %f\n",nextEnemy->velocity.x,nextEnemy->velocityDecay.x);
                if (nextEnemy->velocity.x > 0.1) {
                    nextEnemy->velocity.x -= nextEnemy->velocityDecay.x;
                } else if (nextEnemy->velocity.x < -0.1) {
                    nextEnemy->velocity.x += nextEnemy->velocityDecay.x;
                } else if (nextEnemy->velocity.x < 0.1 && nextEnemy->velocity.x > -0.1) {
                    debug("Reset velocity\n");
                    nextEnemy->state = ENEMY_STATE_PATH;
                    nextEnemy->velocity.x = ENEMY_DEFAULT_VELOCITY;
                    nextEnemy->velocityDecay.x = 0;
                    nextEnemy->velocityDecay.y = 0;
                }
                nextEnemy->position.x += nextEnemy->velocity.x * self->deltaTime;
                //nextEnemy->position.y += nextEnemy->velocity.y * self->deltaTime;
                break;
        }

        projectile_t** collisions = _getProjectileOnEnemyCollisions(self, nextEnemy);

        for (int cIndex=0;cIndex<ENGINE_MAX_PROJECTILES;cIndex++) {
            projectile_t* collidedProjectile = collisions[cIndex];
            if (collidedProjectile == NULL) {
                continue;
            }

            if (collidedProjectile->type == PROJECTILE_TYPE_ENEMY_ROCKET) {
                continue;
            }

            int i = engineGetProjectileIndex(self, collidedProjectile);
            int whichSfx = -1;

            if (i >= 0) {
                if (collidedProjectile->type == PROJECTILE_TYPE_PUNCH) {
                    whichSfx = (rand()%2)+1;
                    nextEnemy->velocity.x = 2.0f;
                    nextEnemy->velocityDecay.x = 0.1;
                    nextEnemy->health -= collidedProjectile->damage;
                    nextEnemy->state = ENEMY_STATE_PUNCHED;
                } else if (collidedProjectile->type == PROJECTILE_TYPE_DEFAULT) {
                    nextEnemy->health -= collidedProjectile->damage;
                    whichSfx = -1;
                }

                if (whichSfx >= 0) {
                    sfxPlay(self->sfx[whichSfx]);
                }

                self->projectiles[i] = NULL;
                projectileDestroy(collidedProjectile);
            }
        }
        free(collisions);

        // Fire a projectile if enough time has passed
        _insertEnemyRocketProjectile(self,nextEnemy);
        activeEnemies++;
    }
    info("%d active enemies\n",activeEnemies);
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
        if (nextProj->updateFunction != NULL) {
            nextProj->updateFunction(nextProj, (void*)self);
        }
        activeProjectiles++;
    }
    info("%d active projectiles\n",activeProjectiles);
}

void _linearProjectileUpdate(projectile_t* self, void* _engine) {
    engine_t* engine = (engine_t*)_engine;
    self->position.x += self->velocity.x * engine->deltaTime;
    self->position.y += self->velocity.y * engine->deltaTime;
}

// RENDER ======================================================================

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
    if (self->player->state == PLAYER_STATE_DEAD) {
        return;
    }
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

void _renderTfx(engine_t* self) {
    for (int tIndex=0; tIndex < ENGINE_MAX_TFX; tIndex++) {
        text_t *nextTfx = self->tfx[tIndex];
        if (nextTfx == NULL) {
            continue;
        }

        SDL_Surface *surface = TTF_RenderText_Solid(nextTfx->font,nextTfx->text,nextTfx->colour);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        SDL_SetTextureAlphaMod(texture,nextTfx->colour.a);
        if (texture == NULL){
            error("Error: Could not create tfx texture\n");
            return;
        }
        int iW, iH;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest;
        dest.x = nextTfx->position.x;
        dest.y = nextTfx->position.y;
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
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

void _renderHUD(engine_t* self) {
    _renderScore(self,1);
    _renderHealth(self,2);
    _renderCoinCount(self,3);
}

void _renderScore(engine_t* self, int i) {
    if (self->scoreText != NULL) {
        snprintf(self->scoreText->text,self->scoreText->bufferSize, "%.6d",self->player->score);
        SDL_Surface *surface = TTF_RenderText_Solid(self->scoreText->font,self->scoreText->text,self->scoreText->colour);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create score texture\n");
            return;
        }
        int iW, iH;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest;
        dest.x = (self->screenWidth/(ENGINE_NUM_HUD_ELEMENTS+1)*1) - (iW/2);
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void _renderHealth(engine_t* self, int i) {
    if (self->healthText != NULL && self->healthSprite != NULL) {
        // Text
        snprintf(self->healthText->text, self->healthText->bufferSize, "%.3d",self->player->health);
        SDL_Surface *surface = TTF_RenderText_Solid(self->healthText->font, self->healthText->text, self->healthText->colour);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create coins text texture\n");
            return;
        }
        int iW, iH;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest, *src;
        dest.x = (self->screenWidth/(ENGINE_NUM_HUD_ELEMENTS+1)*i)-(iW/2);
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // Sprite
        src = spriteGetCurrentFrameRect(self->healthSprite);
        dest.w = self->healthSprite->tileDimensions.x;
        dest.h = self->healthSprite->tileDimensions.y;
        dest.x = dest.x - self->healthSprite->tileDimensions.x-5;
        dest.y = dest.y+(iH/2)-(dest.h/2);
        SDL_RenderCopy(self->renderer, self->healthSprite->texture, src, &dest);
    }
}

void _renderCoinCount(engine_t* self, int i) {
    if (self->coinsText != NULL && self->coinSprite != NULL) {
        // Text
        snprintf(self->coinsText->text, self->coinsText->bufferSize, "%.3d",self->player->numCoins);
        SDL_Surface *surface = TTF_RenderText_Solid(self->coinsText->font, self->coinsText->text, self->coinsText->colour);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create coins text texture\n");
            return;
        }
        int iW, iH;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest, *src;
        dest.x = (self->screenWidth/(ENGINE_NUM_HUD_ELEMENTS+1)*i)-(iW/2);
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // Sprite
        src = spriteGetCurrentFrameRect(self->coinSprite);
        dest.w = self->coinSprite->tileDimensions.x;
        dest.h = self->coinSprite->tileDimensions.y;
        dest.x = dest.x - self->coinSprite->tileDimensions.x-5;
        dest.y = dest.y+(iH/2)-(dest.h/2);
        SDL_RenderCopy(self->renderer, self->coinSprite->texture, src, &dest);
    }
}

void _renderPunchCount(engine_t* self, int i) {
    if (self->punchText != NULL && self->punchSprite != NULL) {
        // Text
        snprintf(self->punchText->text, self->punchText->bufferSize, "%.1d",self->player->numPunches);
        SDL_Surface *surface = TTF_RenderText_Solid(self->punchText->font, self->punchText->text, self->punchText->colour);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create coins text texture\n");
            return;
        }
        int iW, iH;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest, *src;
        dest.x = (self->screenWidth/(ENGINE_NUM_HUD_ELEMENTS+1)*i)-(iW/2);
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // Sprite
        src = spriteGetCurrentFrameRect(self->punchSprite);
        dest.w = self->punchSprite->tileDimensions.x;
        dest.h = self->punchSprite->tileDimensions.y;
        dest.x = dest.x - self->punchSprite->tileDimensions.x-5;
        dest.y = dest.y+(iH/2)-(dest.h/2);
        SDL_RenderCopy(self->renderer, self->punchSprite->texture, src, &dest);
    }
}

// MISC ========================================================================

void _explodePlayer(engine_t* self) {
    for (int vIndex = 0; vIndex < ENGINE_MAX_VFX; vIndex++) {
        if (self->vfx[vIndex] == NULL) {
            sprite_t* expSprite = _createExplosionSprite(self);
            vfx_t *explosion = vfxAllocate(expSprite,VFX_TYPE_PLAYER_EXPLOSION);

            int xPos = (self->player->position.x+self->player->sprite->tileDimensions.x/2)-(expSprite->tileDimensions.x/2);
            explosion->position.x = xPos;
            int yPos = (self->player->position.y+self->player->sprite->tileDimensions.y/2)-(expSprite->tileDimensions.y/2);
            explosion->position.y = yPos;

            explosion->velocity.x = 0;//self->player->velocity.x;
            explosion->velocity.y = 0;//self->player->velocity.y;
            explosion->updateFunction = &_vfxLinearPath;

            self->vfx[vIndex] = explosion;
            sfxPlay(self->sfx[SFX_EXPLOSION_1]);

            break;
        }
    }
}


void _explodeEnemy(engine_t* self, enemy_t* enemy) {
    for (int vIndex = 0; vIndex < ENGINE_MAX_VFX; vIndex++) {
        if (self->vfx[vIndex] == NULL) {
            sprite_t* expSprite = _createExplosionSprite(self);
            vfx_t *explosion = vfxAllocate(expSprite,VFX_TYPE_ENEMY_EXPLOSION);

            int xPos = (enemy->position.x+enemy->sprite->tileDimensions.x/2)-(expSprite->tileDimensions.x/2);
            explosion->position.x = xPos;
            int yPos = (enemy->position.y+enemy->sprite->tileDimensions.y/2)-(expSprite->tileDimensions.y/2);
            explosion->position.y = yPos;

            explosion->velocity.x = enemy->velocity.x;
            explosion->velocity.y = enemy->velocity.y;
            explosion->updateFunction = &_vfxLinearPath;

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
    engine->player->health = 100;
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
    engine->sfx[SFX_COIN_GET   ] = sfxAllocate("res/sfx/coin.wav", 0);
    // Load BGM
    engine->bgm = musicAllocate("res/bgm/japanism.wav",-1);
    musicPlay(engine->bgm);
    // HUD Elements
    engine->scoreText   = textAllocate("res/fonts/nineteen.ttf",32,20);

    engine->healthText  = textAllocate("res/fonts/nineteen.ttf",32,5);
    engine->healthSprite = spriteAllocate("res/gfx/health.png",engine->renderer);

    engine->coinsText   = textAllocate("res/fonts/nineteen.ttf",32,10);
    engine->coinSprite  = spriteAllocate("res/gfx/coin_icon.png",engine->renderer);

    engine->punchText   = textAllocate("res/fonts/nineteen.ttf",32,10);
    engine->punchSprite = spriteAllocate("res/gfx/punch_icon.png",engine->renderer);
    // Done
    return ENGINE_OK;
}

// CREATE ======================================================================
sprite_t* _createExplosionSprite(engine_t* engine) {
    sprite_t* explosion = spriteAllocateSpriteSheet("res/gfx/explosion_2.png",128,128,16,10,engine->renderer);
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
    explosion->frameOrder[4].x = 0;
    explosion->frameOrder[4].y = 1;
    // 5
    explosion->frameOrder[5].x = 1;
    explosion->frameOrder[5].y = 1;
    // 6
    explosion->frameOrder[6].x = 2;
    explosion->frameOrder[6].y = 1;
    // 7
    explosion->frameOrder[7].x = 3;
    explosion->frameOrder[7].y = 1;

    // 8
    explosion->frameOrder[8].x = 0;
    explosion->frameOrder[8].y = 2;
    // 9
    explosion->frameOrder[9].x = 1;
    explosion->frameOrder[9].y = 2;
    // 10
    explosion->frameOrder[10].x = 2;
    explosion->frameOrder[10].y = 2;
    // 11
    explosion->frameOrder[11].x = 3;
    explosion->frameOrder[11].y = 2;

    // 12
    explosion->frameOrder[12].x = 0;
    explosion->frameOrder[12].y = 3;
    // 13
    explosion->frameOrder[13].x = 1;
    explosion->frameOrder[13].y = 3;
    // 14
    explosion->frameOrder[14].x = 2;
    explosion->frameOrder[14].y = 3;
    // 15
    explosion->frameOrder[15].x = 3;
    explosion->frameOrder[15].y = 3;

    return explosion;
}

sprite_t* _createBoomerangSprite(engine_t* engine) {
    sprite_t* boomerangSprite = spriteAllocateSpriteSheet("res/gfx/boomerang.png",32,32,8,15,engine->renderer);
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

int _insertCoinCollectable(engine_t *self, vector2i_t pos) {
    for (int cIndex = 0; cIndex < ENGINE_MAX_COLLECTABLES; cIndex++) {
        if (self->collectables[cIndex] == NULL) {

            sprite_t* coinSprite = _createCoinSprite(self);
            collectable_t* coin = collectableAllocate(coinSprite, COLLECTABLE_TYPE_COIN);

            coin->position.x = pos.x;
            coin->position.y = pos.y;

            self->collectables[cIndex] = coin;
            return cIndex;
        }
    }
    return -1;
}

void _fireDefaultProjectile(engine_t* self) {
    // Has enough time passed
    if (SDL_GetTicks() < self->player->lastProjectile + self->player->projectileDelay) {
        return;
    }

    vector2i_t pos;
    pos.x = self->player->position.x+self->player->sprite->dimensions.x;

    // Top
    pos.y = self->player->position.y + (self->player->sprite->dimensions.y/2) - 10;
    _insertDefaultProjectile(self,pos);

    pos.y = self->player->position.y + (self->player->sprite->dimensions.y/2);
    _insertDefaultProjectile(self,pos);

    pos.y = self->player->position.y + (self->player->sprite->dimensions.y/2) + 10;
    _insertDefaultProjectile(self,pos);

}

int _insertDefaultProjectile(engine_t* self,vector2i_t pos) {
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            //sprite_t* projectileSprite = _createDefaultSprite(self);
            sprite_t* sprite = spriteAllocate("res/gfx/red_projectile.png",self->renderer);

            projectile_t *projectile = projectileAllocate(sprite);

            projectile->position.x = pos.x;
            projectile->position.y = pos.y;


            projectile->velocity.x = PROJECTILE_DEFAULT_VELOCITY_X;
            projectile->velocity.y = PROJECTILE_DEFAULT_VELOCITY_Y;

            self->projectiles[projIndex] = projectile;
            self->player->lastProjectile = SDL_GetTicks();

            projectile->type = PROJECTILE_TYPE_DEFAULT;
            projectile->updateFunction = &_linearProjectileUpdate;
            projectile->damage = 2;

            return projIndex;
        }
    }
    return -1;
}

int _insertEnemyRocketProjectile(engine_t* self, enemy_t* enemy) {
    // Fire a projectile if enough time has passed
    if (self->currentTime < enemy->lastProjectile + enemy->projectileDelay/2 + (rand() % (int)enemy->projectileDelay/2)) {
        return -1;
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

            rocket->velocity.x = ENEMY_ROCKET_PROJECTILE_VELOCITY_X;
            rocket->velocity.y = ENEMY_ROCKET_PROJECTILE_VELOCITY_Y; 
            rocket->damage = 25;
            self->projectiles[projIndex] = rocket;
            rocket->type = PROJECTILE_TYPE_ENEMY_ROCKET;
            rocket->updateFunction = &_linearProjectileUpdate;
            enemy->lastProjectile = self->currentTime;
            return projIndex;
        }
    }
    return -1;
}

int _insertPunchProjectile(engine_t* self) {
    // Has enough time passed
    if (SDL_GetTicks() < self->player->lastPunch + self->player->punchDelay) {
        return -1;
    }
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* fistSprite = spriteAllocate("res/gfx/punch.png",self->renderer);
            projectile_t *fist = projectileAllocate(fistSprite);
            fist->position.x = self->player->position.x+self->player->sprite->dimensions.x;
            fist->position.y =
                self->player->position.y + (self->player->sprite->dimensions.y/2) -
                fistSprite->tileDimensions.y/2;
            fist->velocity.x = PROJECTILE_PUNCH_VELOCITY_X;
            fist->velocity.y = PROJECTILE_PUNCH_VELOCITY_Y;
            fist->type = PROJECTILE_TYPE_PUNCH;
            fist->updateFunction = &_linearProjectileUpdate;
            fist->damage = 1000;
            self->projectiles[projIndex] = fist;
            self->player->lastPunch = SDL_GetTicks();
            self->player->numPunches--;
            self->player->numCoins -= PROJECTILE_PUNCH_COST;
            return projIndex;
        }
    }
    return -1;
}

void _popupTextFloat(text_t* self, void *vEngine) {
    if (self != NULL) {
        engine_t* engine = (engine_t*)vEngine;
        self->position.y -= 0.25f*engine->deltaTime;
        int alphaDec = 5;
        if (self->colour.a > alphaDec)  {
            self->colour.a -= alphaDec;
        } else {
            self->colour.a = 0;
        }
    }
}

int _insertPopupText(engine_t* self, char* text, vector2i_t position) {
    for (int tIndex=0; tIndex<ENGINE_MAX_TFX; tIndex++) {
        if (self->tfx[tIndex] == NULL) {
            text_t* tfx = textAllocate("res/fonts/nineteen.ttf",32,strlen(text)+1);
            if (tfx != NULL) {
                snprintf(tfx->text,tfx->bufferSize,"%s",text);
                tfx->position = position;
                tfx->colour.r = 255;
                tfx->colour.g = 0;
                tfx->colour.b = 0;
                tfx->colour.a = 255;
                tfx->updateFunction = &_popupTextFloat;
                self->tfx[tIndex] = tfx;
                return tIndex;
            } else {
                return -1;
            }
        }
    }
    return -1;
}
