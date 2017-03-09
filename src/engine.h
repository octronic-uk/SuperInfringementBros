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

typedef struct {
    int up;
    int down;
    int left;
    int right;
    int start;
    int btn1;
    int btn2;
    int btn3;
    int btn4;
    int btn5;
    int btn6;
    int btn7;
    int btn8;
} joy_config_t;

typedef struct {
    char name[4];
    int  score;
} highscore_t;


typedef struct engine engine_t;

struct engine {
    // Stuff
    char  state;
    int   screenWidth;
    int   screenHeight;
    char *screenTitle;

    // SDL Objects
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Event    *event;
    SDL_Joystick *joystick;

    // Timing
    float currentTime;
    float lastTime;
    float deltaTime;
    float lastEnemySpawn;
    float enemySpawnDelay;

    // Engine functions
    int (*setupFunction  )(engine_t*);
    int (*renderFunction )(engine_t*);
    int (*updateFunction )(engine_t*);
    int (*cleanupFunction)(engine_t*);

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

    // High Score Screen
    background_t *highScoreBackground;
    text_t      **highScoreTextArray;
    text_t       *highScorePromptText;
    text_t       *highScoreTitle;    
    highscore_t  *highScores;

    // Input
    joy_config_t *joystickConf;
    char upPressed;
    char downPressed;
    char leftPressed;
    char rightPressed;
    char startPressed;
    char btn1Pressed;
    char btn2Pressed;
    char btn3Pressed;
    char btn4Pressed;
    char btn5Pressed;
    char btn6Pressed;
    char btn7Pressed;
    char btn8Pressed;

    // Save Data
    FILE *configFile;
    
};

engine_t* engineAllocate();

int  engineInit(engine_t* self, int width, int height, char* title);
int  engineInitSDL(engine_t* self);

void engineSetState(engine_t*, char);

void engineDestroy(engine_t* self);
void engineDestroySDL(engine_t* self);
void engineCloseSDL(engine_t* self);

int engineInputHandler(engine_t* self);

int  engineLoop(engine_t* self);

int  engineGetProjectileIndex(engine_t* self, projectile_t* projectile);

void engineRemoveAllProjectiles(engine_t* self);
void engineRemoveAllCollectables(engine_t* self) ;
void engineRemoveAllEnemies(engine_t* self);
projectile_t**  engineGetProjectileOnEnemyCollisions(engine_t* self, enemy_t* enemy);

int titleSetupHandler(engine_t* self);
int titleInputHandler(engine_t* self);
int titleUpdateHandler(engine_t* self);
int titleRenderHandler(engine_t* self);
int titleCleanupHandler(engine_t* self);

int gameSetupHandler(engine_t* self);
int gameInputHandler(engine_t* self);
int gameUpdateHandler(engine_t* self);
int gameRenderHandler(engine_t* self);
int gameCleanupHandler(engine_t* self);

int highScoreSetupHandler(engine_t* self);
int highScoreInputHandler(engine_t* self);
int highScoreUpdateHandler(engine_t* self);
int highScoreRenderHandler(engine_t* self);
int highScoreCleanupHandler(engine_t* self);

int menuSetupHandler(engine_t* self);
int menuInputHandler(engine_t* self);
int menuUpdateHandler(engine_t* self);
int menuRenderHandler(engine_t* self);
int menuCleanupHandler(engine_t* self);

#endif // ENGINE_H
