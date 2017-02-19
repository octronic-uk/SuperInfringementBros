#include <stdio.h>
#include <SDL2/SDL.h>
#include "engine.h"
#include "constants.h"

 int main(int argc, char** argv) {
    int retval = 0;
    // Init & Setup
    engine_t *engine = engineAllocate();
    engine->debug = 0;
    int initResult = engineInit(engine,1280 ,720, "SuperInfringementBros!");

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
