#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer *renderer;
    void (*inputFunction)(float);
    void (*renderFunction)(float);
    void (*updateFunction)(float);
} engine_t;

engine_t* engineAllocate();
int       engineInit(engine_t* self, int width, int height, char* title);
void      engineDestroy(engine_t* self);
void      engineLoop(engine_t* self);
