#include <stdio.h>
#include "engine.h"

int main(int argc, char** argv) {
    int retval = 0;
    engine_t *engine = engineAllocate();
    engineInit(engine,800,600,"SuperInfringementBros!");
    engineLoop(engine);
    engineDestroy(engine);
    return retval;
}
