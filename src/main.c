#include <SDL2/SDL.h>

#include "game.h"
#include "title.h"
#include "high_score.h"
#include "menu.h"

#include "engine.h"

#include "constants.h"
#include "logger.h"

 int main(int argc, char** argv) {
    // Init & Setup
    engine_t *engine = engineAllocate();
    int initResult = engineInit(engine, 1280 ,720, "SuperInfringementBros!");

    if (initResult == ENGINE_ERROR) {
        error("Engine init failed\n");
        return 1;
    }
    // Set title state
    engineSetState(engine,ENGINE_STATE_GAME);
    // Run
    int retval = engineLoop(engine);
    // Cleanup
    engineDestroy(engine);
    return retval;
}
