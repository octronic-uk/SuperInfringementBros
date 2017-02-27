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
#include "collectable.h"

typedef struct engine engine_t;

struct engine {
    char  debug;
    int   screenWidth;
    int   screenHeight;
    char *screenTitle;
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
    player_t       *player;
    background_t  **backgrounds;
    sfx_t         **sfx;
    projectile_t  **projectiles;
    enemy_t       **enemies;
    vfx_t         **vfx;
    music_t        *bgm;
    collectable_t **collectables;
    text_t        **tfx;
    // HUD
    text_t         *scoreText;
    sprite_t       *coinSprite;
    text_t         *coinsText;
    sprite_t       *punchSprite;
    text_t         *punchText;
    sprite_t       *healthSprite;
    text_t         *healthText;
        // Input
    char upPressed;
    char downPressed;
    char leftPressed;
    char rightPressed;
    char pauseBtnPressed;
    char fire1BtnPressed;
    char fire2BtnPressed;
    char buyBtnPressed;
    char weaponBtnPressed;
};

engine_t* engineAllocate();
int  engineInit(engine_t* self, int width, int height, char* title);
void engineDestroy(engine_t* self);
int  engineLoop(engine_t* self);
int  engineDefaultInputHandler(engine_t* self);
int  engineDefaultUpdateHandler(engine_t* self);
int  engineDefaultRenderHandler(engine_t* self);
int  engineGetProjectileIndex(engine_t* self, projectile_t* projectile);
void engineDestroyResources(engine_t* self);
void engineDestroySDL(engine_t* self);
int  engineInitSDL(engine_t* self);
void engineCloseSDL(engine_t* self);

// 'Private' Functions 

// Setup/Init
int       _setupResources(engine_t* self);

// Create: Create assets for ...
sprite_t* _createBoomerangSprite(engine_t* engine);
sprite_t* _createEnemyRocketSprite(engine_t* engine);
sprite_t* _createCoinSprite(engine_t* engine);
sprite_t* _createExplosionSprite(engine_t* engine);
enemy_t*  _createEnemy(engine_t* engine, int numEnemies, int i);

// Insert: Insert into runtime ...
int _insertPopupText(engine_t* self, char* font, int size, char* text, vector2i_t position, int r, int g, int b, int a);
int _insertPunchProjectile(engine_t* self);
int _insertBoomerangProjectile(engine_t* self);
int _insertEnemyRocketProjectile(engine_t* self, enemy_t* enemy);
int _insertCoinCollectable(engine_t *self, vector2i_t pos);

void _explodeEnemy(engine_t* self, enemy_t* enemy);

// Render
void _renderBackgrounds(engine_t* self);
void _renderPlayer(engine_t* self);
void _renderEnemies(engine_t* self);
void _renderProjectiles(engine_t* self);
void _renderVfx(engine_t *self);
void _renderCollectables(engine_t* self);

void _renderHUD(engine_t *self);
void _renderScore(engine_t* self, int i);
void _renderHealth(engine_t* self, int i);
void _renderCoinCount(engine_t* self, int i);
void _renderPunchCount(engine_t* self, int i);

// Update 
void _updateBackgrounds(engine_t* self);
void _updatePlayer(engine_t* self);
void _updateEnemies(engine_t* self);
void _updateProjectiles(engine_t* self);
void _updateVfx(engine_t* self);
void _updateCollectables(engine_t *self);

projectile_t**  _getProjectileOnEnemyCollisions(engine_t* self, enemy_t* enemy);

#endif // ENGINE_H
