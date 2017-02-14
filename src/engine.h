#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer *renderer;
    void (*inputFunction)();
    void (*logicFunction)();
    void (*updateFunction)();
} engine_t;

engine_t* engineAllocate();
int       engineInitVideo(engine_t* self, int width, int height, char* title);
void      engineDestroy(engine_t* self);
void      engineLoop(engine_t* self);
