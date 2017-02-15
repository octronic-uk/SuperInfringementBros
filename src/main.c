#include <stdio.h>
#include <SDL2/SDL.h>
#include "engine.h"
 
int setup(engine_t* engine) {
    // Setup Player
    sprite_t* playerSprite = spriteAllocate("res/mario.png",engine->renderer);
    engine->player = playerAllocate(playerSprite);
    engine->player->velocity.x = 20;
    engine->player->velocity.y = 10;
    // Setup Backgrounds
    background_t *bg = backgroundAllocate("res/sky.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 0;
        bg->repeatScroll = 0;
        engine->backgrounds[0] = bg;
    }
    bg = backgroundAllocate("res/clouds.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -1;
        engine->backgrounds[1] = bg;
    }
    bg = backgroundAllocate("res/clouds_2.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -2;
        engine->backgrounds[2] = bg;
    }
    bg = backgroundAllocate("res/ground.png",engine->renderer);
    if (bg!=NULL) {
        bg->scroll = 1;
        bg->repeatScroll = 1;
        bg->velocity.x = -4;
        engine->backgrounds[3] = bg;
    }
    return ENGINE_OK;
}

int main(int argc, char** argv) {
    int retval = 0;
    // Init & Setup
    engine_t *engine = engineAllocate();
    int initResult = engineInit(engine,800,512,"SuperInfringementBros!");

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
