#include "engine.h"

engine_t* engineAllocate() {

    engine_t* engine    = (engine_t*)malloc(sizeof(engine_t));
    engine->event       = (SDL_Event*)malloc(sizeof(SDL_Event));
    engine->backgrounds = (background_t*)malloc(sizeof(background_t*)*MAX_BACKGROUNDS);

    engine->currentTime = 0.0f;
    engine->lastTime    = 0.0f;
    engine->deltaTime   = 0.0f;;

    engine->window         = NULL;
    engine->renderer       = NULL;

    engine->inputFunction  = NULL;
    engine->updateFunction = NULL;
    engine->renderFunction = NULL;

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
    
    for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
        background_t *nextBg = (self->backgrounds+bgIndex);
        nextBg  = NULL; 
    }

    return 0;
}

void engineDestroy(engine_t* self) {

    if (self->renderer != NULL) {
        SDL_DestroyRenderer(self->renderer);
    }

    if (self->window != NULL) {
        SDL_DestroyWindow(self->window);
    }

    if (self->backgrounds != NULL) {
        for (int bgIndex = 0; bgIndex < MAX_BACKGROUNDS; bgIndex++) {
            background_t* nextBg = (self->backgrounds+bgIndex);
            if (nextBg != NULL) {
                backgroundDestroy(nextBg);
                nextBg = NULL;
            }
        }
        self->backgrounds = NULL;
    }

    SDL_Quit();
}

int engineLoop(engine_t* self) {
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
    return ENGINE_OK;
}

int engineDefaultInputHandler(engine_t* self) { 
    SDL_PollEvent(self->event);
    if (self->event->type == SDL_QUIT) {
        return ENGINE_QUIT;
    } else if (self->event->type == SDL_KEYDOWN) {
        // KeyDown
    } else if (self->event->type == SDL_KEYUP) {
        // KeyUp
    } else if (self->event->type == SDL_MOUSEBUTTONDOWN) {
        // MouseButtonDown
    } else if (self->event->type == SDL_MOUSEBUTTONUP) {
        // MouseButtonUp
    }
    return ENGINE_OK;
}

int engineDefaultUpdateHandler(engine_t* self) { 
    return ENGINE_OK;
}  

int engineDefaultRenderHandler(engine_t* self) {
    return ENGINE_OK;
}  
