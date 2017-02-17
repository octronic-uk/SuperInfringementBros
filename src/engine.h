#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_Mixer.h>
#include "background.h"
#include "player.h"
#include "projectile.h"
#include "sfx.h"
#include "enemy.h"

#define ENGINE_ERROR -1
#define ENGINE_OK     0
#define ENGINE_QUIT   1

#define MAX_BACKGROUNDS 5 
#define MAX_SFX 10
#define MAX_PROJECTILES 100
#define MAX_ENEMIES 50

typedef struct engine engine_t;

struct engine {
    char debug;
    int screenWidth;
    int screenHeight;
    // SDL Objects
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Event    *event;
    // Timing
    float currentTime;
    float lastTime;
    float deltaTime;
    // Engine functions
    int (*inputFunction )(engine_t*);
    int (*renderFunction)(engine_t*);
    int (*updateFunction)(engine_t*);
    // Engine things
    player_t      *player;
    projectile_t **projectiles;
    sfx_t        **sfx;
    background_t **backgrounds;
    enemy_t      **enemies;
    // Projectlie Vars
    float lastProjectile;
    float projectileDelay;
    // Input
    char upPressed;
    char downPressed;
    char leftPressed;
    char rightPressed;
    char aBtnPressed;
    char bBtnPressed;
};

engine_t* engineAllocate();
int  engineInit(engine_t* self, int width, int height, char* title);
void engineDestroy(engine_t* self);
int  engineLoop(engine_t* self);
int  engineDefaultInputHandler(engine_t* self);
int  engineDefaultUpdateHandler(engine_t* self);
int  engineDefaultRenderHandler(engine_t* self);


// 'Private' Functions 
int       _setupResources(engine_t* self);
sprite_t* _createFireballSprite(engine_t* engine);
void      _spawnProjectile(engine_t* self);

#endif // ENGINE_H
