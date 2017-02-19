#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_Mixer.h>
#include <SDL2/SDL_ttf.h>
#include "background.h"
#include "player.h"
#include "projectile.h"
#include "sfx.h"
#include "music.h"
#include "enemy.h"
#include "text.h"
#include "vfx.h"

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
    background_t **backgrounds;
    sfx_t        **sfx;
    projectile_t **projectiles;
    enemy_t      **enemies;
    vfx_t        **vfx;
    music_t       *bgm;
    text_t       *score;
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
int engineGetProjectileIndex(engine_t* self, projectile_t* projectile);

// 'Private' Functions 

// Setup/Init
int       _setupResources(engine_t* self);

sprite_t* _createBoomerangSprite(engine_t* engine);
sprite_t* _createEnemyRocketSprite(engine_t* engine);
sprite_t* _createCoinSprite(engine_t* engine);
sprite_t* _createExplosionSprite(engine_t* engine);

enemy_t*  _createEnemy(engine_t* engine, int numEnemies, int i);

void _createPunchProjectile(engine_t* self);
void _createBoomerangProjectile(engine_t* self);
void _createEnemyRocketProjectile(engine_t* self, enemy_t* enemy);

void _explodeEnemy(engine_t* self, enemy_t* enemy);

// Render
void _renderBackgrounds(engine_t* self);
void _renderPlayer(engine_t* self);
void _renderEnemies(engine_t* self);
void _renderProjectiles(engine_t* self);
void _renderScore(engine_t* self);
void _renderVfx(engine_t *self);

// Update 
void _updateBackgrounds(engine_t* self);
void _updatePlayer(engine_t* self);
void _updateEnemies(engine_t* self);
void _updateProjectiles(engine_t* self);
void _updateVfx(engine_t* self);

projectile_t** _getProjectileOnEnemyCollisions(engine_t* self, enemy_t* enemy);

#endif // ENGINE_H
