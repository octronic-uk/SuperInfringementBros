#include "engine.h"
#include <stdio.h>

engine_t* engineAllocate() {
    printf("Allocating new engine_t\n");
    engine_t* engine    = (engine_t*)malloc(sizeof(engine_t));
    engine->event       = (SDL_Event*)malloc(sizeof(SDL_Event));
    // Malloc Backgrounds Array
    engine->backgrounds = (background_t**)malloc(sizeof(background_t*)*MAX_BACKGROUNDS);
    for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
        engine->backgrounds[bgIndex] = NULL; 
    }
    // Malloc Projectiles Array
    engine->projectiles = (projectile_t**)malloc(sizeof(projectile_t*)*MAX_PROJECTILES);
    for (int pIndex = 0; pIndex < MAX_PROJECTILES; pIndex++) {
        engine->projectiles[pIndex] = NULL; 
    }
    // Malloc SFX Array
    engine->sfx = (sfx_t**)malloc(sizeof(sfx_t*)*MAX_SFX);
    for (int sIndex = 0; sIndex < MAX_SFX; sIndex++) {
        engine->sfx[sIndex] = NULL; 
    }
    // Malloc Enemy Array
    engine->enemies = (enemy_t**)malloc(sizeof(enemy_t*)*MAX_ENEMIES);
    for (int eIndex = 0; eIndex < MAX_ENEMIES; eIndex++) {
        engine->enemies[eIndex] = NULL; 
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
    engine->projectileDelay = DEFAULT_PROJECTILE_DELAY;

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

    // open 44.1KHz, signed 16bit, system byte order,
    //      stereo audio, using 1024 byte chunks
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
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
            for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
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
            for (int sIndex=0;sIndex<MAX_SFX;sIndex++) {
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
            for (int pIndex=0;pIndex<MAX_PROJECTILES;pIndex++) {
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
            for (int eIndex=0; eIndex<MAX_ENEMIES; eIndex++) {
                if (self->enemies[eIndex] != NULL) {
                    enemyDestroy(self->enemies[eIndex]);
                    self->enemies[eIndex] = NULL;
                }
            }
            free(self->enemies);
            self->enemies = NULL;
        }
    }
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
            case SDLK_SPACE:
                self->aBtnPressed = 1;
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
            case SDLK_SPACE:
                self->aBtnPressed = 0;
                break;
        }
    } 
    return ENGINE_OK;
}

int engineDefaultUpdateHandler(engine_t* self) { 
    if (self->debug)  {
        printf("DeltaTime: %.2fms\n",self->deltaTime);
    }

    // Scroll Backgrounds
    for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {

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

    if (self->aBtnPressed) {
        _spawnProjectile(self);
    }

    // Update Enemies
    for (int eIndex = 0; eIndex < MAX_ENEMIES; eIndex++) {
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
        // Needs Update
        nextEnemy->position.x += nextEnemy->velocity.x*self->deltaTime;
        nextEnemy->position.y += nextEnemy->velocity.y*self->deltaTime;
    }

    // Update Projectiles
    for (int projIndex = 0; projIndex < MAX_PROJECTILES; projIndex++) {
        projectile_t *nextProj = self->projectiles[projIndex];
        // Not set
        if (nextProj == NULL) {
            continue;
        } 
        // Out of bounds
        if (nextProj->position.x > self->screenWidth) {
            projectileDestroy(nextProj);
            self->projectiles[projIndex] = NULL;
            continue;
        }
        // Needs Update
        nextProj->position.x += nextProj->velocity.x*self->deltaTime;
        nextProj->position.y += nextProj->velocity.y*self->deltaTime;
    }

    // Done
    return ENGINE_OK;
}  

int engineDefaultRenderHandler(engine_t* self) {
    // Clear
    SDL_RenderClear(self->renderer);
    // Render Backgrounds
    for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
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
    // Render Player
    SDL_Rect playerDest;
    playerDest.w = self->player->sprite->dimensions.x;
    playerDest.h = self->player->sprite->dimensions.y;
    playerDest.x = self->player->position.x;
    playerDest.y = self->player->position.y;
    SDL_RenderCopy(self->renderer,self->player->sprite->texture,NULL,&playerDest);
    // Render Enemies
    for (int eIndex=0; eIndex < MAX_ENEMIES; eIndex++) {
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
    // Render Projectiles
    for (int projIndex=0; projIndex < MAX_PROJECTILES; projIndex++) {
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
    // Present
    SDL_RenderPresent(self->renderer);
    return ENGINE_OK;
}  


int _setupResources(engine_t* engine) {
    // Setup Player
    sprite_t* playerSprite = spriteAllocate("res/gfx/ship_128.png",engine->renderer);
    engine->player = playerAllocate(playerSprite);
    engine->player->position.x = 20;
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
        bg->velocity.x = -0.4f;
        engine->backgrounds[3] = bg;
    }
    // Allocate Fireball SFX
    engine->sfx[0] = sfxAllocate("res/sfx/shoot.wav",0);
    // Setup Enemies
    sprite_t* enemySprite = spriteAllocate("res/gfx/enemy.png",engine->renderer);
    enemy_t* enemy = enemyAllocate(enemySprite,NULL);
    enemy->position.x = engine->screenWidth - enemy->sprite->dimensions.x*2;
    enemy->position.y = engine->screenHeight/2 - enemy->sprite->dimensions.y/2;
    enemy->velocity.x = 0.0f;
    enemy->velocity.y = 0.0f;
    engine->enemies[0] = enemy;
    // Done
    return ENGINE_OK;
}

sprite_t* _createFireballSprite(engine_t* engine) {
    // Projectile
    sprite_t* fireballSprite = spriteAllocateSpriteSheet("res/gfx/boomerang.png",32,32,8,25,engine->renderer);
    // Animation Frames
    // 0 
    fireballSprite->frameOrder[7].x = 0;
    fireballSprite->frameOrder[7].y = 0;
    // 1
    fireballSprite->frameOrder[6].x = 1;
    fireballSprite->frameOrder[6].y = 0;
    // 2
    fireballSprite->frameOrder[5].x = 2;
    fireballSprite->frameOrder[5].y = 0;
    // 3
    fireballSprite->frameOrder[4].x = 3;
    fireballSprite->frameOrder[4].y = 0;
    // 4
    fireballSprite->frameOrder[3].x = 4;
    fireballSprite->frameOrder[3].y = 0;
    // 5 
    fireballSprite->frameOrder[2].x = 5;
    fireballSprite->frameOrder[2].y = 0;
    // 6 
    fireballSprite->frameOrder[1].x = 6;
    fireballSprite->frameOrder[1].y = 0;
    // 7 
    fireballSprite->frameOrder[0].x = 7;
    fireballSprite->frameOrder[0].y = 0;


    return fireballSprite;
}

void _spawnProjectile(engine_t* self) {
    // Has enough time passed
    if (SDL_GetTicks() < self->lastProjectile + self->projectileDelay) {
        return;
    }
    // New Projectile
    for (int projIndex = 0; projIndex < MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* fireballSprite = _createFireballSprite(self);
            projectile_t *fireball = projectileAllocate(fireballSprite);
            fireball->position.x = self->player->position.x+self->player->sprite->dimensions.x+8;
            fireball->position.y = 
                self->player->position.y + (self->player->sprite->dimensions.y/2) -
                fireballSprite->tileDimensions.y/2;
            fireball->velocity.x = DEFAULT_PROJECTILE_VELOCITY_X;
            fireball->velocity.y = DEFAULT_PROJECTILE_VELOCITY_Y;
            self->projectiles[projIndex] = fireball;
            self->lastProjectile = SDL_GetTicks();
            sfxPlay(self->sfx[0]);
            break;
        }
    }
}
