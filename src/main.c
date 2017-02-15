#include <stdio.h>
#include <SDL2/SDL.h>
#include "engine.h"

 int main(int argc, char** argv) {
    int retval = 0;
    // Init & Setup
    engine_t *engine = engineAllocate();
    engine->debug = 0;
    int initResult = engineInit(engine,800,512,"SuperInfringementBros!");

    if (initResult != 0) {
        printf("Engine init failed\n");
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
