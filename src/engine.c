#include "engine.h"
#include <stdio.h>

engine_t* engineAllocate() {
    printf("Allocating new engine_t\n");
    engine_t* engine    = (engine_t*)malloc(sizeof(engine_t));
    engine->event       = (SDL_Event*)malloc(sizeof(SDL_Event));
    engine->player      = playerAllocate(); 
    engine->backgrounds = (background_t**)malloc(sizeof(background_t*)*MAX_BACKGROUNDS);

    for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
        engine->backgrounds[bgIndex] = NULL; 
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
            case SDLK_UP:
                self->upPressed = 1;
                break;
            case SDLK_DOWN:
                self->upPressed = 1;
                break;
            case SDLK_LEFT:
                self->upPressed = 1;
                break;
            case SDLK_RIGHT:
                self->upPressed = 1;
                break;
        }
    } else if (self->event->type == SDL_KEYUP) {
        // KeyUp
        switch (self->event->key.keysym.sym) {
            case SDLK_UP:
                self->upPressed = 0;
                break;
            case SDLK_DOWN:
                self->upPressed = 0;
                break;
            case SDLK_LEFT:
                self->upPressed = 0;
                break;
            case SDLK_RIGHT:
                self->upPressed = 0;
                break;
        }
    } 
    return ENGINE_OK;
}

int engineDefaultUpdateHandler(engine_t* self) { 
    for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {

        background_t *nextBg = self->backgrounds[bgIndex];

        if (nextBg == NULL) {
            continue;
        } 

        nextBg->position.x += nextBg->velocity.x;
        nextBg->position.y += nextBg->velocity.y;

        if (nextBg->position.x < -(nextBg->dimensions.x/2)) {
            nextBg->position.x = -1;
        }

        if (nextBg->position.y < -(nextBg->dimensions.y/2)) {
            nextBg->position.y = -1;
        }

    }

    return ENGINE_OK;
}  

int engineDefaultRenderHandler(engine_t* self) {
    SDL_RenderClear(self->renderer);
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
    SDL_RenderPresent(self->renderer);
    return ENGINE_OK;
}  
