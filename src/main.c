#include <stdio.h>
#include <SDL2/SDL.h>
#include "engine.h"
 
int setup(engine_t* engine) {
    background_t *bg = backgroundAllocate("res/bg1.png",engine->renderer);
    if (bg!=NULL) {
        bg->velocity.x = -1;
        engine->backgrounds[0] = bg;
    }
    return ENGINE_OK;
}

int main(int argc, char** argv) {
    int retval = 0;
    // Init & Setup
    engine_t *engine = engineAllocate();
    int initResult = engineInit(engine,800,600,"SuperInfringementBros!");

    if (initResult != 0) {
        printf("Engine init failed\n");
        return ENGINE_ERROR;
    }

    if (setup(engine) != ENGINE_OK) {
        printf("Error during setup\n");
        return ENGINE_ERROR;
    }
    // Set engine methods
    engine->inputFunction  = &engineDefaultInputHandler;
    engine->updateFunction = &engineDefaultUpdateHandler;
    engine->renderFunction = &engineDefaultRenderHandler;
    // Run
    retval = engineLoop(engine);
    // Cleanup
    engineDestroy(engine);
    return retval;
}
