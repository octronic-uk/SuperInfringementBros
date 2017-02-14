#include "engine.h"

engine_t* engineAllocate() {
    engine_t* engine = (engine_t*)malloc(sizeof(engine_t));
    engine->window = NULL;
    engine->renderer = NULL;
    engine->inputFunction = NULL;
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

    return 0;
}

void engineDestroy(engine_t* self) {

    if (self->renderer != NULL) {
        SDL_DestroyRenderer(self->renderer);
    }

    if (self->window != NULL) {

        SDL_DestroyWindow(self->window);
    }

    SDL_Quit();
}

void engineLoop(engine_t* self) {
    char  done        = 0;
    float currentTime = 0.0f;
    float lastTime    = 0.0f;
    float deltaTime   = 0.0f;;

    do {
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime;

        if (self->inputFunction != NULL) {
            self->inputFunction(deltaTime);
        } else {
            done = 1;
        }

        if (self->updateFunction != NULL) {
            self->updateFunction(deltaTime);
        } else {
            done = 1;
        }

        if (self->renderFunction != NULL) {
            self->renderFunction(deltaTime);
        } else { 
            done = 1;
        }

        lastTime = currentTime;
    } while (done == 0);
}
