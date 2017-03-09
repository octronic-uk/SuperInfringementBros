#include <stdio.h>
#include "engine.h"
#include "constants.h"
#include "logger.h"

engine_t* engineAllocate() {
    debug("Allocating new engine_t\n");
    engine_t* engine = (engine_t*)malloc(sizeof(engine_t));
    engine->event    = (SDL_Event*)malloc(sizeof(SDL_Event));
    engine->joystick = NULL;

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

    // Setup Handlers
    engine->setupFunction   = NULL;
    engine->updateFunction  = NULL;
    engine->renderFunction  = NULL;
    engine->cleanupFunction = NULL;

    // Event variables
    engine->upPressed    = 0;
    engine->downPressed  = 0;
    engine->leftPressed  = 0;
    engine->rightPressed = 0;

    engine->btn1Pressed = 0;
    engine->btn2Pressed = 0;
    engine->btn3Pressed = 0;
    engine->btn4Pressed = 0;
    engine->btn5Pressed = 0;
    engine->btn6Pressed = 0;
    engine->btn7Pressed = 0;
    engine->btn8Pressed = 0;

    engine->startPressed = 0;

    engine->state = ENGINE_STATE_NONE;

    // High Score Variables
    engine->highScoreTextArray  = (text_t**)malloc(sizeof(text_t*)*HIGH_SCORES_MAX);
    engine->highScoreTitle      = NULL;
    engine->highScoreBackground = NULL;
    engine->highScores          = (highscore_t*)malloc(sizeof(highscore_t)*HIGH_SCORES_MAX);

    return engine;
}

void engineSetState(engine_t* self, char state) {
    if (self != NULL) {
        // cleanup previous state
        if (self->cleanupFunction != NULL) {
            self->cleanupFunction(self);
            self->state = ENGINE_STATE_NONE;
        }
        // Set state functions
        switch (state) {
            case ENGINE_STATE_TITLE:
                self->setupFunction   = &titleSetupHandler;
                self->updateFunction  = &titleUpdateHandler;
                self->renderFunction  = &titleRenderHandler;
                self->cleanupFunction = &titleCleanupHandler;
                break;
            case ENGINE_STATE_GAME:
                self->setupFunction   = &gameSetupHandler;
                self->updateFunction  = &gameUpdateHandler;
                self->renderFunction  = &gameRenderHandler;
                self->cleanupFunction = &gameCleanupHandler;
                break;
            case ENGINE_STATE_HIGHSCORE:
                self->setupFunction   = &highScoreSetupHandler;
                self->updateFunction  = &highScoreUpdateHandler;
                self->renderFunction  = &highScoreRenderHandler;
                self->cleanupFunction = &highScoreCleanupHandler;
                break;
            case ENGINE_STATE_MENU:
                self->setupFunction   = &menuSetupHandler;
                self->updateFunction  = &menuUpdateHandler;
                self->renderFunction  = &menuRenderHandler;
                self->cleanupFunction = &menuCleanupHandler;
                break;
            default:
                self->setupFunction   = NULL;
                self->updateFunction  = NULL;
                self->renderFunction  = NULL;
                self->cleanupFunction = NULL;
                break;
        }
        // Setup next state
        if (self->setupFunction != NULL) {
            if (self->setupFunction(self) == ENGINE_OK) {
                self->state = state;
            } else {
                self->state = ENGINE_STATE_ERROR;
            }
        }
    }
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

    return ENGINE_OK;
}

int engineInitSDL(engine_t* self) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0){
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

    self->window = SDL_CreateWindow(self->screenTitle, 0, 0, self->screenWidth, self->screenHeight, SDL_WINDOW_SHOWN || SDL_WINDOW_FULLSCREEN);

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

    debug("Joysticks Connected: %d\n",SDL_NumJoysticks());

    if (SDL_NumJoysticks() > 0) {
        self->joystick = SDL_JoystickOpen(0);
        if (self->joystick == NULL) {
            error("Error opening joystick %s\n",SDL_GetError());
        }
    }
    return ENGINE_OK;
}

void engineDestroy(engine_t* self) {
    debug("Destroying engine_t\n");
    if (self != NULL) {
        // Clean up active state
        if (self->cleanupFunction != NULL) {
            self->cleanupFunction(self);
        }
        // High Scores Array
        if (self->highScores != NULL) {
            free(self->highScores);
            self->highScores = NULL;
        }
        // Joystic Config
        if (self->joystickConf != NULL) {
            free(self->joystickConf);
            self->joystickConf = NULL;
        }
        engineDestroySDL(self);
        engineCloseSDL(self);
        free(self);
    }
}

void engineDestroySDL(engine_t* self) {
    // Delete SDL Objects
    if (self->joystick != NULL) {
        SDL_JoystickClose(self->joystick);
        self->joystick = NULL;
    }
    if (self->renderer != NULL) {
        SDL_DestroyRenderer(self->renderer);
        self->renderer = NULL;
    }
    if (self->window != NULL) {
        SDL_DestroyWindow(self->window);
        self->window = NULL;
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

        if (engineInputHandler(self) == ENGINE_QUIT) {
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
            SDL_RenderClear(self->renderer);
            int result = self->renderFunction(self);
            SDL_RenderPresent(self->renderer);
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

void engineRemoveAllEnemies(engine_t* self) {
    for (int eIndex=0; eIndex<ENGINE_MAX_ENEMIES; eIndex++) {
        if (self->enemies[eIndex] != NULL) {
            enemyDestroy(self->enemies[eIndex]);
            self->enemies[eIndex] = NULL;
        }
    }
}

void engineRemoveAllCollectables(engine_t* self) {
    for (int cIndex=0; cIndex<ENGINE_MAX_COLLECTABLES; cIndex++) {
        if (self->collectables[cIndex] != NULL) {
            collectableDestroy(self->collectables[cIndex]);
            self->collectables[cIndex] = NULL;
        }
    }
}

void engineRemoveAllProjectiles(engine_t* self) {
    for (int pIndex=0; pIndex<ENGINE_MAX_PROJECTILES; pIndex++) {
        if (self->projectiles[pIndex] != NULL) {
            projectileDestroy(self->projectiles[pIndex]);
            self->projectiles[pIndex] = NULL;
        }
    }
}

int engineGetProjectileIndex(engine_t *self, projectile_t* projectile) {
    for (int pIndex = 0; pIndex<ENGINE_MAX_PROJECTILES; pIndex++){
        if (self->projectiles[pIndex] == projectile) {
            return pIndex;
        }
    }
    return -1;
}

projectile_t** engineGetProjectileOnEnemyCollisions(engine_t* engine, enemy_t* enemy) {
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

int engineInputHandler(engine_t* self) {
    while(SDL_PollEvent(self->event)) {
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
                    self->btn1Pressed = 1;
                    break;
                case SDLK_s:
                    self->btn2Pressed = 1;
                    break;
                case SDLK_d:
                    self->btn3Pressed = 1;
                    break;
                case SDLK_f:
                    self->btn4Pressed = 1;
                    break;
                case SDLK_RETURN:
                    self->startPressed = 1;
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
                    self->btn1Pressed = 0;
                    break;
                case SDLK_s:
                    self->btn2Pressed = 0;
                    break;
                case SDLK_d:
                    self->btn3Pressed = 0;
                    break;
                case SDLK_f:
                    self->btn4Pressed = 0;
                    break;
                case SDLK_RETURN:
                    self->startPressed = 0;
                    break;
            }
        }  else if (self->event->type == SDL_JOYAXISMOTION) {
            // X
            if (self->event->jaxis.axis == 0) {
                if (self->event->jaxis.value < -8000) {
                    self->leftPressed = 1;
                    self->rightPressed = 0;

                } else if (self->event->jaxis.value > 8000) {
                    self->leftPressed = 0;
                    self->rightPressed = 1;

                } else {
                    self->leftPressed = 0;
                    self->rightPressed = 0;
                }
            }
            // Y
            else if (self->event->jaxis.axis == 1) {
                if (self->event->jaxis.value < -8000) {
                    self->upPressed = 1;
                    self->downPressed = 0;

                } else if (self->event->jaxis.value > 8000) {
                    self->upPressed = 0;
                    self->downPressed = 1;

                } else {
                    self->upPressed = 0;
                    self->downPressed = 0;
                }
            }
        } else if (self->event->type == SDL_JOYHATMOTION) {
            switch (self->event->jhat.value) {
                case SDL_HAT_UP:
                    self->upPressed   = 1;
                    self->downPressed = 0;
                    break;
                case SDL_HAT_DOWN:
                    self->upPressed   = 0;
                    self->downPressed = 1;
                    break;
                case SDL_HAT_LEFT:
                    self->leftPressed  = 1;
                    self->rightPressed = 0;
                    break;
                case SDL_HAT_RIGHT:
                    self->leftPressed  = 0;
                    self->rightPressed = 1;
                    break;
                case SDL_HAT_CENTERED:
                    self->upPressed    = 0;
                    self->downPressed  = 0;
                    self->leftPressed  = 0;
                    self->rightPressed = 0;
                    break;
            }
        } else if (self->event->type == SDL_JOYBUTTONDOWN) {
            switch(self->event->jbutton.button) {
                case 0:
                    self->btn1Pressed = 1;
                    break;
                case 1:
                    self->btn2Pressed = 1;
                    break;
                case 8:
                    self->startPressed = 1;
                    break;
            }
        } else if (self->event->type == SDL_JOYBUTTONUP) {
            switch(self->event->jbutton.button) {
                case 0:
                    self->btn1Pressed = 0;
                    break;
                case 1:
                    self->btn2Pressed = 0;
                    break;
                case 8:
                    self->startPressed = 0;
                    break;
            }
        }
    }
    return ENGINE_OK;
}

