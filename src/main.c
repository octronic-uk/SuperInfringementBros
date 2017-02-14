#include <stdio.h>
#include <SDL2/SDL.h>
#include "engine.h"
 
int setup() {
    return ENGINE_OK;
}

int render(engine_t* engine) {

}

int update(engine_t* engine) {

}

int input(engine_t* engine) {
    SDL_PollEvent(engine->event);
    if (engine->event->type == SDL_QUIT) {
        return ENGINE_QUIT;
    } else if (engine->event->type == SDL_KEYDOWN) {
        // KeyDown
    } else if (engine->event->type == SDL_KEYUP) {
        // KeyUp
    } else if (engine->event->type == SDL_MOUSEBUTTONDOWN) {
        // MouseButtonDown
    } else if (engine->event->type == SDL_MOUSEBUTTONUP) {
        // MouseButtonUp
    }
    return ENGINE_OK;
}

int main(int argc, char** argv) {
    int retval = 0;
    // Init & Setup
    engine_t *engine = engineAllocate();
    engineInit(engine,800,600,"SuperInfringementBros!");
    if (setup() != ENGINE_OK) {
        printf("Error during setup\n");
        return ENGINE_ERROR;
    }
    // Set engine methods
    engine->inputFunction  = &input;
    engine->updateFunction = &update;
    engine->renderFunction = &render;
    // Run
    retval = engineLoop(engine);
    // Cleanup
    engineDestroy(engine);
    return retval;
}
