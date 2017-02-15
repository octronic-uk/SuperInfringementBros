#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include "background.h"
#include "player.h"

#define ENGINE_ERROR -1
#define ENGINE_OK     0
#define ENGINE_QUIT   1
#define MAX_BACKGROUNDS 5 

typedef struct engine engine_t;

struct engine {
    int screenWidth;
    int screenHeight;
    // SDL Objects
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Event    *event;
    background_t **backgrounds;
    // Timing
    float currentTime;
    float lastTime;
    float deltaTime;
    // Engine functions
    int (*inputFunction )(engine_t*);
    int (*renderFunction)(engine_t*);
    int (*updateFunction)(engine_t*);
    // Engine things
    player_t *player;
    // Input
    char upPressed;
    char downPressed;
    char leftPressed;
    char rightPressed;
    char aBtnPressed;
    char bBtnPressed;
};

engine_t* engineAllocate();
int       engineInit(engine_t* self, int width, int height, char* title);
void      engineDestroy(engine_t* self);
int       engineLoop(engine_t* self);

// Default engine methods
int engineDefaultInputHandler(engine_t*);
int engineDefaultUpdateHandler(engine_t*);
int engineDefaultRenderHandler(engine_t*);

#endif // ENGINE_H
