#include "engine.h"
#include <stdio.h>

engine_t* engineAllocate() {
    printf("Allocating new engine_t\n");
    engine_t* engine    = (engine_t*)malloc(sizeof(engine_t));
    engine->event       = (SDL_Event*)malloc(sizeof(SDL_Event));

    engine->backgrounds = (background_t**)malloc(sizeof(background_t*)*MAX_BACKGROUNDS);
    for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
        engine->backgrounds[bgIndex] = NULL; 
    }

    engine->projectiles = (projectile_t**)malloc(sizeof(projectile_t)*MAX_PROJECTILES);
    for (int pIndex = 0; pIndex < MAX_PROJECTILES; pIndex++) {
        engine->projectiles[pIndex] = NULL; 
    }

    engine->currentTime = 0.0f;
    engine->lastTime    = 0.0f;
    engine->deltaTime   = 0.0f;;

    engine->window         = NULL;
    engine->renderer       = NULL;

    engine->inputFunction  = NULL;
    engine->updateFunction = NULL;
    engine->renderFunction = NULL;

    engine->upPressed    = 0;
    engine->downPressed  = 0;
    engine->leftPressed  = 0;
    engine->rightPressed = 0;

    engine->aBtnPressed  = 0;
    engine->bBtnPressed  = 0;

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

        if (self->player != NULL) {
            playerDestroy(self->player);
        }

        if (self->renderer != NULL) {
            SDL_DestroyRenderer(self->renderer);
        }

        if (self->window != NULL) {
            SDL_DestroyWindow(self->window);
        }

        if (self->backgrounds != NULL) {
            for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
                if (self->backgrounds[bgIndex] != NULL) {
                    backgroundDestroy(self->backgrounds[bgIndex]);
                    self->backgrounds[bgIndex] = NULL;
                }
            }
            self->backgrounds = NULL;
        }
    }
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
    if (self->debug) 
        printf("DeltaTime: %.2fms\n",self->deltaTime);

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
    if (self->upPressed == 1 ) { 
        if (self->player->position.y > 0) {
            self->player->position.y -= self->player->velocity.y*self->deltaTime;
        } else {
            self->player->position.y = 0;
        }
    }

    if (self->downPressed == 1) {
        if(self->player->position.y < self->screenHeight - self->player->sprite->dimensions.y) {
            self->player->position.y += self->player->velocity.y*self->deltaTime;
        }
        else {
            self->player->position.y = self->screenHeight - self->player->sprite->dimensions.y;
        }
    }

    if (self->leftPressed == 1) { 
        if  (self->player->position.x > 0) {
            self->player->position.x -= self->player->velocity.x*self->deltaTime;
        } else {
            self->player->position.x = 0;
        }
    }

    if (self->rightPressed == 1) {
        if(self->player->position.x < self->screenWidth - self->player->sprite->dimensions.x) {
            self->player->position.x += self->player->velocity.x*self->deltaTime;
        } else {
            self->player->position.x = self->screenWidth - self->player->sprite->dimensions.x;
        }
    }

    if (self->aBtnPressed) {
        _spawnProjectile(self);
    }

    // Update Projectiles
    for (int projIndex = 0; projIndex < MAX_PROJECTILES; projIndex++) {
        projectile_t *nextProj = self->projectiles[projIndex];
        if (nextProj == NULL) {
            continue;
        } 
        if (nextProj->position.x > self->screenWidth) {
            projectileDestroy(nextProj);
            self->projectiles[projIndex] = NULL;
        }
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
    // Render Projectiles
    for (int projIndex=0; projIndex < MAX_PROJECTILES; projIndex++) {
        projectile_t *nextProj = self->projectiles[projIndex];
        if (nextProj == NULL) {
            continue;
        } 
        SDL_Rect src, dest;
        src = spriteGetCurrentFrameRect(nextProj->sprite);
        dest.w = nextProj->sprite->tileDimensions.x;
        dest.h = nextProj->sprite->tileDimensions.y;
        dest.x = nextProj->position.x;
        dest.y = nextProj->position.y;
        SDL_RenderCopy(self->renderer, nextProj->sprite->texture, &src, &dest);
        spriteAdvanceFrame(nextProj->sprite,self->deltaTime);
    }
    // Present
    SDL_RenderPresent(self->renderer);
    return ENGINE_OK;
}  


int _setupResources(engine_t* engine) {
    // Setup Player
    sprite_t* playerSprite = spriteAllocate("res/tiny_ship.png",engine->renderer);
    engine->player = playerAllocate(playerSprite);
    engine->player->velocity.x = 0.5f;
    engine->player->velocity.y = 0.5f;
    engine->player->position.x = 20;
    engine->player->position.y = engine->screenHeight/2-playerSprite->dimensions.y/2;
    // Setup Backgrounds
    background_t *bg = backgroundAllocate("res/sky.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 0;
        bg->repeatScroll = 0;
        engine->backgrounds[0] = bg;
    }
    bg = backgroundAllocate("res/clouds.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -0.1f;
        engine->backgrounds[1] = bg;
    }
    bg = backgroundAllocate("res/clouds_2.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -0.2f;
        engine->backgrounds[2] = bg;
    }
    bg = backgroundAllocate("res/ground.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -0.4f;
        engine->backgrounds[3] = bg;
    }

    return ENGINE_OK;
}

sprite_t* _createFireballSprite(engine_t* engine) {
    // Projectile
    sprite_t* fireballSprite = spriteAllocateSpriteSheet("res/fireball_16.png",16,16,4,50,engine->renderer);
    // Animation Frames
    // 0 
    fireballSprite->frameOrder[0].x = 0;
    fireballSprite->frameOrder[0].y = 0;
    // 1
    fireballSprite->frameOrder[1].x = 1;
    fireballSprite->frameOrder[1].y = 0;
    // 2
    fireballSprite->frameOrder[2].x = 2;
    fireballSprite->frameOrder[2].y = 0;
    // 3
    fireballSprite->frameOrder[3].x = 3;
    fireballSprite->frameOrder[3].y = 0;

    return fireballSprite;
}

void _spawnProjectile(engine_t* self) {
    for (int projIndex = 0; projIndex < MAX_PROJECTILES; projIndex++) {
        if (self->projectiles[projIndex] == NULL) {
            sprite_t* fireballSprite = _createFireballSprite(self);
            projectile_t *fireball = projectileAllocate(fireballSprite);
            fireball->position.x = self->player->position.x+self->player->sprite->dimensions.x+8;
            fireball->position.y = self->player->position.y+self->player->sprite->dimensions.y/2;
            fireball->velocity.x = 0.2f;
            fireball->velocity.y = 0.0f;
            self->projectiles[projIndex] = fireball;
            break;
        }
    }
}
