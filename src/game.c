#include "game.h"
#include "constants.h"

int gameCleanupHandler(engine_t* self) {
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
    return ENGINE_OK;
}

int gameUpdateHandler(engine_t* self) {
    info("DeltaTime: %.2fms\n",self->deltaTime);
    gameUpdateBackgrounds(self);
    gameUpdatePlayer(self);
    gameUpdateProjectiles(self);
    gameUpdateCollectables(self);
    gameUpdateEnemies(self);
    gameUpdateVfx(self);
    gameUpdateTfx(self);
    // Done
    return ENGINE_OK;
}

int gameRenderHandler(engine_t* self) {
    // Clear
    gameRenderBackgrounds(self);
    gameRenderPlayer(self);
    gameRenderEnemies(self);
    gameRenderProjectiles(self);
    gameRenderVfx(self);
    gameRenderCollectables(self);
    gameRenderTfx(self);
    gameRenderHUD(self);
    return ENGINE_OK;
}

int gameSetupHandler(engine_t* engine) {
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
    engine->sfx[SFX_HEALTH_GET ] = sfxAllocate("res/sfx/health.wav", 0);

    // Load BGM
    engine->bgm = musicAllocate("res/bgm/guile.wav",-1);
    //engine->bgm = musicAllocate("res/bgm/world_bowser.wav",-1);
    musicPlay(engine->bgm);
    // HUD Elements
    engine->scoreText   = textAllocate("res/fonts/nineteen.ttf",DEFAULT_FONT_SIZE,20);

    engine->healthText  = textAllocate("res/fonts/nineteen.ttf",DEFAULT_FONT_SIZE,5);
    engine->healthSprite = spriteAllocate("res/gfx/health.png",engine->renderer);

    engine->coinsText   = textAllocate("res/fonts/nineteen.ttf",DEFAULT_FONT_SIZE,10);
    engine->coinSprite  = spriteAllocate("res/gfx/coin_icon.png",engine->renderer);

    engine->punchText   = textAllocate("res/fonts/nineteen.ttf",DEFAULT_FONT_SIZE,10);
    engine->punchSprite = spriteAllocate("res/gfx/punch_icon.png",engine->renderer);
    // Done
    return ENGINE_OK;
}

// UPDATE ======================================================================

void gameUpdateBackgrounds(engine_t* self) {
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


void gameUpdatePlayer(engine_t* self) {
    // Player exists
    if (self->player != NULL) {
        if (self->player->state == PLAYER_STATE_DEAD) {
            if (self->startPressed){
                self->player->score = 0;
                self->player->health = 100;
                self->player->numCoins = 0;
                self->player->position.x = self->player->sprite->dimensions.x;
                self->player->position.y = self->screenHeight/2-self->player->sprite->dimensions.y/2;
                self->player->state = PLAYER_STATE_ALIVE;
                engineRemoveAllProjectiles(self);
                engineRemoveAllEnemies(self);
                engineRemoveAllCollectables(self);
            } else {
                return;
            }
        }
        // U dead?
        if (self->player->health <= 0) {
            gameExplodePlayer(self);     
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
                gameExplodeEnemy(self,enemy);
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

        if (self->btn1Pressed == 1) {
            gameFireDefaultProjectile(self);
        }
        if (self->btn2Pressed == 1) {
            if (self->player->numCoins >= PROJECTILE_PUNCH_COST) {
                gameInsertPunchProjectile(self);
            }
        }

    }
}

void gameUpdateTfx(engine_t* self) {
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

void gameUpdateVfx(engine_t* self) {
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
                if (self->player->health < 100) {
                    int coll = rand() % 10;
                    if (coll == 0) {
                        gameInsertHealthCollectable(self,pos);
                    } else {
                        gameInsertCoinCollectable(self,pos);
                    }
                } else {
                    gameInsertCoinCollectable(self,pos);
                }
            }
            vfxDestroy(nextVfx);
            self->vfx[vIndex] = NULL;
        }
    }
}

void gameUpdateCollectables(engine_t* self) {
    vector2i_t pPosition   = self->player->position;
    vector2i_t pDimensions = self->player->sprite->tileDimensions;

    for (int cIndex=0; cIndex<ENGINE_MAX_COLLECTABLES; cIndex++) {
        collectable_t *nextCol = self->collectables[cIndex];
        if (nextCol == NULL) {
            continue;
        }

        // Off Screen
        if (nextCol->position.x < 0 - nextCol->sprite->dimensions.x || nextCol->position.x > self->screenWidth  || 
                nextCol->position.y < 0 - nextCol->sprite->dimensions.y || nextCol->position.y > self->screenHeight ) {
            collectableDestroy(nextCol);
            self->collectables[cIndex] = NULL;
            continue;
        }

        vector2i_t cPosition   = nextCol->position;
        vector2i_t cDimensions = nextCol->sprite->tileDimensions;

        if (vector2iCollision(cPosition, cDimensions, pPosition, pDimensions) == 1) {
            switch (nextCol->type) {
                case COLLECTABLE_TYPE_COIN:
                    collectableDestroy(nextCol);
                    self->collectables[cIndex] = NULL;
                    sfxPlay(self->sfx[SFX_COIN_GET]);
                    self->player->numCoins++;
                    self->player->score += 100;
                    continue;
                    break;
                case COLLECTABLE_TYPE_HEALTH:
                    collectableDestroy(nextCol);
                    self->collectables[cIndex] = NULL;
                    sfxPlay(self->sfx[SFX_HEALTH_GET]);
                    self->player->health+=50;
                    if (self->player->health > 100) {
                        self->player->health = 100;
                    }
                    continue;
                    break;
            }
        }

        // Update position
        nextCol->position.x += nextCol->velocity.x*self->deltaTime;
        nextCol->position.y += nextCol->velocity.y*self->deltaTime;
    }
}

void gameVfxLinearPath(vfx_t* self, void* vEngine) {
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
}

void gameEnemyLinearPath(enemy_t* enemy, void* vEngine) {
    enemy->position.x += enemy->velocity.x * ((engine_t*)vEngine)->deltaTime;
}

void gameRockFireProjectileFunction(enemy_t* enemy, void* _engine) { 
    engine_t* self = (engine_t*) _engine;
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* rocketSprite = gameCreateYinYangSprite(self);
            projectile_t *rocket = projectileAllocate(rocketSprite);
            rocket->position.x = enemy->position.x;
            rocket->position.y = enemy->position.y + (enemy->sprite->dimensions.y/2) - rocketSprite->tileDimensions.y/2;

            rocket->velocity.x = ENEMY_ROCKET_PROJECTILE_VELOCITY_X;
            rocket->velocity.y = ENEMY_ROCKET_PROJECTILE_VELOCITY_Y; 

            rocket->damage = 25;
            self->projectiles[projIndex] = rocket;
            rocket->type = PROJECTILE_TYPE_ENEMY_ROCKET;
            rocket->updateFunction = &gameLinearProjectileUpdate;
            enemy->lastProjectile = self->currentTime;
            return;
        }
    }
}

sprite_t* gameCreateLemonSprite(engine_t* self) {
    sprite_t* proj = spriteAllocateSpriteSheet("res/gfx/lemon.png",32,32,4,50,self->renderer);
    proj->frameOrder[0].x = 0;
    proj->frameOrder[0].y = 0;

    proj->frameOrder[1].x = 1;
    proj->frameOrder[1].y = 0;

    proj->frameOrder[2].x = 2;
    proj->frameOrder[2].y = 0;

    proj->frameOrder[3].x = 3;
    proj->frameOrder[3].y = 0;
    return proj;
}

sprite_t* gameCreateRGBYSprite(engine_t* self) {
    sprite_t* proj = spriteAllocateSpriteSheet("res/gfx/rgby_proj.png",17,17,4,50,self->renderer);
    proj->frameOrder[0].x = 0;
    proj->frameOrder[0].y = 0;

    proj->frameOrder[1].x = 1;
    proj->frameOrder[1].y = 0;

    proj->frameOrder[2].x = 2;
    proj->frameOrder[2].y = 0;

    proj->frameOrder[3].x = 3;
    proj->frameOrder[3].y = 0;
    return proj;
}

sprite_t* gameCreateYinYangSprite(engine_t* self) {
    sprite_t* proj = spriteAllocateSpriteSheet("res/gfx/blue_red_proj.png",32,32,8,25,self->renderer);
    proj->frameOrder[0].x = 0;
    proj->frameOrder[0].y = 0;

    proj->frameOrder[1].x = 1;
    proj->frameOrder[1].y = 0;

    proj->frameOrder[2].x = 2;
    proj->frameOrder[2].y = 0;

    proj->frameOrder[3].x = 3;
    proj->frameOrder[3].y = 0;

    proj->frameOrder[4].x = 4;
    proj->frameOrder[4].y = 0;

    proj->frameOrder[5].x = 5;
    proj->frameOrder[5].y = 0;

    proj->frameOrder[6].x = 6;
    proj->frameOrder[6].y = 0;

    proj->frameOrder[7].x = 7;
    proj->frameOrder[7].y = 0;
    return proj;
}

void gameBlobFireProjectileFunction(enemy_t* enemy, void* _engine) { 
    engine_t* self = (engine_t*) _engine;
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* rocketSprite = gameCreateRGBYSprite(self);
            projectile_t *rocket = projectileAllocate(rocketSprite);
            rocket->position.x = enemy->position.x;
            rocket->position.y = enemy->position.y + (enemy->sprite->dimensions.y/2) - rocketSprite->tileDimensions.y/2;

            rocket->velocity.x = ENEMY_ROCKET_PROJECTILE_VELOCITY_X*1.25;

            int posDelta =  rocket->position.y - self->player->position.y;
            if (posDelta > 50) {
                rocket->velocity.y = -0.1f; 
            } else if (posDelta < -50) {
                rocket->velocity.y = 0.1f; 
            } else {
                rocket->velocity.y = 0.0f; 
            }

            rocket->damage = 25;
            self->projectiles[projIndex] = rocket;
            rocket->type = PROJECTILE_TYPE_ENEMY_ROCKET;
            rocket->updateFunction = &gameLinearProjectileUpdate;
            enemy->lastProjectile = self->currentTime;
            return;
        }
    }
}

void gameIcyFireProjectileFunction(enemy_t* enemy, void* _engine) {
    engine_t* self = (engine_t*) _engine;
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* rocketSprite = gameCreateLemonSprite(self);
            projectile_t *rocket = projectileAllocate(rocketSprite);
            rocket->position.x = enemy->position.x;
            rocket->position.y = enemy->position.y + (enemy->sprite->dimensions.y/2) - rocketSprite->tileDimensions.y/2;

            rocket->velocity.x = ENEMY_ROCKET_PROJECTILE_VELOCITY_X*1.75;

            int posDelta =  rocket->position.y - self->player->position.y;
            if (posDelta > 50) {
                rocket->velocity.y = -0.1f; 
            } else if (posDelta < -50) {
                rocket->velocity.y = 0.1f; 
            } else {
                rocket->velocity.y = 0.0f; 
            }


            rocket->damage = 25;
            self->projectiles[projIndex] = rocket;
            rocket->type = PROJECTILE_TYPE_ENEMY_ROCKET;
            rocket->updateFunction = gameLinearProjectileUpdate;
            enemy->lastProjectile = self->currentTime;
            return;
        }
    }
}

void gameSaucerFireProjectileFunction(enemy_t* enemy, void* _engine) {
    engine_t* self = (engine_t*) _engine;
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* rocketSprite = gameCreateEnemyRocketSprite(self);
            projectile_t *rocket = projectileAllocate(rocketSprite);
            rocket->position.x = enemy->position.x;
            rocket->position.y = enemy->position.y + (enemy->sprite->dimensions.y/2) - rocketSprite->tileDimensions.y/2;

            rocket->velocity.x = ENEMY_ROCKET_PROJECTILE_VELOCITY_X*1.5;
            rocket->velocity.y = ENEMY_ROCKET_PROJECTILE_VELOCITY_Y; 

            rocket->damage = 25;
            self->projectiles[projIndex] = rocket;
            rocket->type = PROJECTILE_TYPE_ENEMY_ROCKET;
            rocket->updateFunction = gameLinearProjectileUpdate;
            enemy->lastProjectile = self->currentTime;
            return;
        }
    }
}

void gameSpawnEnemy(engine_t* self) {
    if (self->currentTime > self->lastEnemySpawn + self->enemySpawnDelay) {
        for (int eIndex=0; eIndex<ENGINE_MAX_ENEMIES; eIndex++) {
            if (self->enemies[eIndex] == NULL) {

                int enemyNum = rand()%4;
                void* projFunction = NULL;
                sprite_t* enemySprite;                 
                switch (enemyNum) {
                    case 0:
                        enemySprite = spriteAllocate("res/gfx/enemy_1.png",self->renderer);
                        projFunction = gameSaucerFireProjectileFunction;
                        break;
                    case 1:
                        enemySprite = spriteAllocate("res/gfx/enemy_2.png",self->renderer);
                        projFunction = gameIcyFireProjectileFunction;
                        break;
                    case 2:
                        enemySprite = spriteAllocate("res/gfx/enemy_3.png",self->renderer);
                        projFunction = gameBlobFireProjectileFunction;
                        break;
                    case 3:
                        enemySprite = spriteAllocate("res/gfx/enemy_4.png",self->renderer);
                        projFunction = gameRockFireProjectileFunction;
                        break;
                }

                enemy_t* enemy = enemyAllocate(enemySprite);

                int spriteHeight = enemy->sprite->dimensions.y;
                int spriteWidth = enemy->sprite->dimensions.x;

                enemy->position.x = self->screenWidth;
                enemy->position.y = rand() % (self->screenHeight - spriteHeight*2) + spriteHeight;

                enemy->velocity.x = -0.1;
                enemy->velocity.y = 0.0f;

                enemy->health = 100;
                enemy->state = ENEMY_STATE_PATH;
                enemy->fireProjectileFunction = projFunction;
                enemy->updateFunction = gameEnemyLinearPath;
                self->enemies[eIndex] = enemy;
                self->lastEnemySpawn = self->currentTime;
                break;
            }
        }
    }
}

void gameUpdateEnemies(engine_t* self) {
    gameSpawnEnemy(self);
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
            gameInsertPopupText(self,"1000",nextEnemy->position);
            self->player->score += 1000;
            gameExplodeEnemy(self,nextEnemy);
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

        projectile_t** collisions = engineGetProjectileOnEnemyCollisions(self, nextEnemy);

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
                    collidedProjectile->collisions += 1;
                    if (collidedProjectile->collisions > 1) {
                        char points[50];
                        snprintf(&points[0],50,"%d",collidedProjectile->collisions*10000);
                        self->player->score += collidedProjectile->collisions*10000;
                        gameInsertPopupText(self,points,collidedProjectile->position);
                    }
                } else if (collidedProjectile->type == PROJECTILE_TYPE_DEFAULT) {
                    nextEnemy->health -= collidedProjectile->damage;
                    self->projectiles[i] = NULL;
                    projectileDestroy(collidedProjectile);
                    whichSfx = -1;
                }

                if (whichSfx >= 0) {
                    sfxPlay(self->sfx[whichSfx]);
                }
            }
        }
        free(collisions);

        // Fire a projectile if enough time has passed
        gameInsertEnemyProjectile(self,nextEnemy);
        activeEnemies++;
    }
    info("%d active enemies\n",activeEnemies);
}

void gameUpdateProjectiles(engine_t* self) {
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

void gameLinearProjectileUpdate(projectile_t* self, void* _engine) {
    engine_t* engine = (engine_t*)_engine;
    self->position.x += self->velocity.x * engine->deltaTime;
    self->position.y += self->velocity.y * engine->deltaTime;
}

void gameRenderBackgrounds(engine_t* self) {
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

void gameRenderPlayer(engine_t* self) {
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

void gameRenderEnemies(engine_t* self) {
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

void gameRenderTfx(engine_t* self) {
    for (int tIndex=0; tIndex < ENGINE_MAX_TFX; tIndex++) {
        text_t *nextTfx = self->tfx[tIndex];
        if (nextTfx == NULL) {
            continue;
        }
        SDL_Color black;
        black.r = 0;
        black.g = 0;
        black.b = 0;
        black.a = nextTfx->colour.a;
        // Stroke
        TTF_SetFontOutline(nextTfx->font, 4);
        SDL_Surface *surface = TTF_RenderText_Solid(nextTfx->font,nextTfx->text,black);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        SDL_SetTextureAlphaMod(texture,nextTfx->colour.a);
        if (texture == NULL){
            error("Error: Could not create tfx texture\n");
            return;
        }
        int iW, iH, cX, cY;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest;
        dest.x = nextTfx->position.x;
        dest.y = nextTfx->position.y;
        dest.w = iW;
        dest.h = iH;
        cX = dest.x + (iW/2);
        cY = dest.y + (iH/2);
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // Text
        TTF_SetFontOutline(nextTfx->font, 0);
        surface = TTF_RenderText_Solid(nextTfx->font,nextTfx->text,nextTfx->colour);
        texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        SDL_SetTextureAlphaMod(texture,nextTfx->colour.a);
        if (texture == NULL){
            error("Error: Could not create tfx texture\n");
            return;
        }
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        dest.x = cX - (iW/2);
        dest.y = cY - (iH/2);
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void gameRenderVfx(engine_t* self) {
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

void gameRenderCollectables(engine_t* self) {
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

void gameRenderProjectiles(engine_t* self) {
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

void gameRenderHUD(engine_t* self) {
    gameRenderScore(self,1);
    gameRenderHealth(self,2);
    gameRenderCoinCount(self,3);
}

void gameRenderScore(engine_t* self, int i) {
    if (self->scoreText != NULL) {
        SDL_Color black;
        black.r = 0;
        black.g = 0;
        black.b = 0;
        black.a = 0;
        // Stroke
        TTF_SetFontOutline(self->scoreText->font, 4);
        snprintf(self->scoreText->text,self->scoreText->bufferSize, "%.6d",self->player->score);
        SDL_Surface *surface = TTF_RenderText_Solid(self->scoreText->font,self->scoreText->text,black);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create score texture\n");
            return;
        }
        int iW, iH, cX, cY;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest;
        dest.x = (self->screenWidth/(ENGINE_NUM_HUD_ELEMENTS+1)*1) - (iW/2);
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        cX = dest.x + (iW/2);
        cY = dest.y + (iH/2);
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // Text
        TTF_SetFontOutline(self->scoreText->font, 0);
        surface = TTF_RenderText_Solid(self->scoreText->font,self->scoreText->text,self->scoreText->colour);
        texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create score texture\n");
            return;
        }
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        dest.x = cX - (iW/2);
        dest.y = cY - (iH/2);
        dest.w = iW;
        dest.h = iH;
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void gameRenderHealth(engine_t* self, int i) {
    if (self->healthText != NULL && self->healthSprite != NULL) {
        SDL_Color black;
        black.r = 0;
        black.g = 0;
        black.b = 0;
        black.a = 0;
        //Stroke 
        TTF_SetFontOutline(self->healthText->font, 4);
        snprintf(self->healthText->text, self->healthText->bufferSize, "%.3d",self->player->health);
        SDL_Surface *surface = TTF_RenderText_Solid(self->healthText->font, self->healthText->text, black);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create coins text texture\n");
            return;
        }
        int iW, iH, cX, cY;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest, *src;
        dest.x = (self->screenWidth/(ENGINE_NUM_HUD_ELEMENTS+1)*i)-(iW/2);
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        cX = dest.x + (iW/2);
        cY = dest.y + (iH/2);
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // Text
        TTF_SetFontOutline(self->healthText->font, 0);
        surface = TTF_RenderText_Solid(self->healthText->font, self->healthText->text, self->healthText->colour);
        texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create coins text texture\n");
            return;
        }
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        dest.x = cX - (iW/2);
        dest.y = cY - (iH/2);
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

void gameRenderCoinCount(engine_t* self, int i) {
    if (self->coinsText != NULL && self->coinSprite != NULL) {
        SDL_Color black;
        black.r = 0;
        black.g = 0;
        black.b = 0;
        black.a = 0;
        // Stroke
        TTF_SetFontOutline(self->coinsText->font, 4);
        snprintf(self->coinsText->text, self->coinsText->bufferSize, "%.3d",self->player->numCoins);
        SDL_Surface *surface = TTF_RenderText_Solid(self->coinsText->font, self->coinsText->text, black);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create coins text texture\n");
            return;
        }
        int iW, iH, cX, cY;
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        SDL_Rect dest, *src;
        dest.x = (self->screenWidth/(ENGINE_NUM_HUD_ELEMENTS+1)*i)-(iW/2);
        dest.y = self->screenHeight-iH-5;
        dest.w = iW;
        dest.h = iH;
        cX = dest.x + (iW/2);
        cY = dest.y + (iH/2);
        SDL_RenderCopy(self->renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        // Text
        TTF_SetFontOutline(self->coinsText->font, 0);
        snprintf(self->coinsText->text, self->coinsText->bufferSize, "%.3d",self->player->numCoins);
        surface = TTF_RenderText_Solid(self->coinsText->font, self->coinsText->text, self->coinsText->colour);
        texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        if (texture == NULL){
            error("Error: Could not create coins text texture\n");
            return;
        }
        SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
        dest.x = cX - (iW/2);
        dest.y = cY - (iH/2);
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

// MISC ========================================================================

void gameExplodePlayer(engine_t* self) {
    for (int vIndex = 0; vIndex < ENGINE_MAX_VFX; vIndex++) {
        if (self->vfx[vIndex] == NULL) {
            sprite_t* expSprite = gameCreateExplosionSprite(self);
            vfx_t *explosion = vfxAllocate(expSprite,VFX_TYPE_PLAYER_EXPLOSION);

            int xPos = (self->player->position.x+self->player->sprite->tileDimensions.x/2)-(expSprite->tileDimensions.x/2);
            explosion->position.x = xPos;
            int yPos = (self->player->position.y+self->player->sprite->tileDimensions.y/2)-(expSprite->tileDimensions.y/2);
            explosion->position.y = yPos;

            explosion->velocity.x = 0;//self->player->velocity.x;
            explosion->velocity.y = 0;//self->player->velocity.y;
            explosion->updateFunction = gameVfxLinearPath;

            self->vfx[vIndex] = explosion;
            sfxPlay(self->sfx[SFX_EXPLOSION_1]);

            break;
        }
    }
}

void gameExplodeEnemy(engine_t* self, enemy_t* enemy) {
    for (int vIndex = 0; vIndex < ENGINE_MAX_VFX; vIndex++) {
        if (self->vfx[vIndex] == NULL) {
            sprite_t* expSprite = gameCreateExplosionSprite(self);
            vfx_t *explosion = vfxAllocate(expSprite,VFX_TYPE_ENEMY_EXPLOSION);

            int xPos = (enemy->position.x+enemy->sprite->tileDimensions.x/2)-(expSprite->tileDimensions.x/2);
            explosion->position.x = xPos;
            int yPos = (enemy->position.y+enemy->sprite->tileDimensions.y/2)-(expSprite->tileDimensions.y/2);
            explosion->position.y = yPos;

            explosion->velocity.x = enemy->velocity.x;
            explosion->velocity.y = enemy->velocity.y;
            explosion->updateFunction = gameVfxLinearPath;

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

// CREATE ======================================================================
sprite_t* gameCreateExplosionSprite(engine_t* engine) {
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

sprite_t* gameCreateBoomerangSprite(engine_t* engine) {
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

sprite_t* gameCreateEnemyRocketSprite(engine_t* engine) {
    sprite_t* rocketSprite = spriteAllocateSpriteSheet("res/gfx/rocket.png",64,23,2,150,engine->renderer);
    // 0
    rocketSprite->frameOrder[0].x = 0;
    rocketSprite->frameOrder[0].y = 0;
    // 1
    rocketSprite->frameOrder[1].x = 1;
    rocketSprite->frameOrder[1].y = 0;

    return rocketSprite;
}

sprite_t* gameCreateCoinSprite(engine_t* self) {
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

int gameInsertHealthCollectable(engine_t *self, vector2i_t pos) {
    for (int cIndex = 0; cIndex < ENGINE_MAX_COLLECTABLES; cIndex++) {
        if (self->collectables[cIndex] == NULL) {
            sprite_t* healthSprite = spriteAllocate("res/gfx/ham.png",self->renderer);
            collectable_t* health = collectableAllocate(healthSprite, COLLECTABLE_TYPE_HEALTH);

            health->position.x = pos.x;
            health->position.y = pos.y;

            health->velocity.x = ENEMY_DEFAULT_VELOCITY; 
            health->velocity.y = 0;

            self->collectables[cIndex] = health;
            return cIndex;
        }
    }
    return -1;
}

int gameInsertCoinCollectable(engine_t *self, vector2i_t pos) {
    for (int cIndex = 0; cIndex < ENGINE_MAX_COLLECTABLES; cIndex++) {
        if (self->collectables[cIndex] == NULL) {

            sprite_t* coinSprite = gameCreateCoinSprite(self);
            collectable_t* coin = collectableAllocate(coinSprite, COLLECTABLE_TYPE_COIN);

            coin->position.x = pos.x;
            coin->position.y = pos.y;

            coin->velocity.x = ENEMY_DEFAULT_VELOCITY; 
            coin->velocity.y = 0;

            self->collectables[cIndex] = coin;
            return cIndex;
        }
    }
    return -1;
}

void gameFireDefaultProjectile(engine_t* self) {
    // Has enough time passed
    if (SDL_GetTicks() < self->player->lastProjectile + self->player->projectileDelay) {
        return;
    }

    vector2i_t pos;
    pos.x = self->player->position.x+self->player->sprite->dimensions.x;

    // Top
    pos.y = self->player->position.y + (self->player->sprite->dimensions.y/2) - 10;
    gameInsertDefaultProjectile(self,pos);

    pos.y = self->player->position.y + (self->player->sprite->dimensions.y/2);
    gameInsertDefaultProjectile(self,pos);

    pos.y = self->player->position.y + (self->player->sprite->dimensions.y/2) + 10;
    gameInsertDefaultProjectile(self,pos);
}

int gameInsertDefaultProjectile(engine_t* self,vector2i_t pos) {
    // New Projectile
    for (int projIndex = 0; projIndex < ENGINE_MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            //sprite_t* projectileSprite = gameCreateDefaultSprite(self);
            sprite_t* sprite = spriteAllocate("res/gfx/red_projectile.png",self->renderer);

            projectile_t *projectile = projectileAllocate(sprite);

            projectile->position.x = pos.x;
            projectile->position.y = pos.y;


            projectile->velocity.x = PROJECTILE_DEFAULT_VELOCITY_X;
            projectile->velocity.y = PROJECTILE_DEFAULT_VELOCITY_Y;

            self->projectiles[projIndex] = projectile;
            self->player->lastProjectile = SDL_GetTicks();

            projectile->type = PROJECTILE_TYPE_DEFAULT;
            projectile->updateFunction = gameLinearProjectileUpdate;
            projectile->damage = 3;

            return projIndex;
        }
    }
    return -1;
}

int gameInsertEnemyProjectile(engine_t* self, enemy_t* enemy) {
    // Fire a projectile if enough time has passed
    if (self->currentTime < enemy->lastProjectile + enemy->projectileDelay/2 + (rand() % (int)enemy->projectileDelay/2)) {
        return -1;
    }
    if (enemy->fireProjectileFunction != NULL) {
        enemy->fireProjectileFunction(enemy,self);
    }
    return 0;
}

int gameInsertPunchProjectile(engine_t* self) {
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
            fist->updateFunction = gameLinearProjectileUpdate;
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

void gamePopupTextFloat(text_t* self, void *vEngine) {
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

int gameInsertPopupText(engine_t* self, char* text, vector2i_t position) {
    for (int tIndex=0; tIndex<ENGINE_MAX_TFX; tIndex++) {
        if (self->tfx[tIndex] == NULL) {
            text_t* tfx = textAllocate("res/fonts/nineteen.ttf",DEFAULT_FONT_SIZE,strlen(text)+1);
            if (tfx != NULL) {
                snprintf(tfx->text,tfx->bufferSize,"%s",text);
                tfx->position = position;
                tfx->colour.r = 255;
                tfx->colour.g = 255;
                tfx->colour.b = 255;
                tfx->colour.a = 255;
                tfx->updateFunction = gamePopupTextFloat;
                self->tfx[tIndex] = tfx;
                return tIndex;
            } else {
                return -1;
            }
        }
    }
    return -1;
}
