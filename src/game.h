#ifndef GAME_H
#define GAME_H

#include "engine.h"
#include "logger.h"

// Handler Functions
int gameSetupHandler  (engine_t* self);
int gameInputHandler  (engine_t* self);
int gameUpdateHandler (engine_t* self);
int gameRenderHandler (engine_t* self);
int gameCleanupHandler(engine_t* self);


// Create Assets Functions
sprite_t* gameCreateBoomerangSprite(engine_t* engine);
sprite_t* gameCreateEnemyRocketSprite(engine_t* engine);
sprite_t* gameCreateCoinSprite(engine_t* engine);
sprite_t* gameCreateExplosionSprite(engine_t* engine);
sprite_t* gameCreateRGBYSprite(engine_t* self);
sprite_t* gameCreateLemonSprite(engine_t* self);

// Insert Asset Functions
int gameInsertPopupText(engine_t* self, char* text, vector2i_t position);
int gameInsertPunchProjectile(engine_t* self);
int gameInsertDefaultProjectile(engine_t* self,vector2i_t pos);
int gameInsertEnemyProjectile(engine_t* self, enemy_t* enemy);
int gameInsertCoinCollectable(engine_t *self, vector2i_t pos);
int gameInsertHealthCollectable(engine_t *self, vector2i_t pos);

// Render Functions
void gameRenderBackgrounds(engine_t* self);
void gameRenderPlayer(engine_t* self);
void gameRenderEnemies(engine_t* self);
void gameRenderProjectiles(engine_t* self);
void gameRenderVfx(engine_t *self);
void gameRenderTfx(engine_t *self);
void gameRenderCollectables(engine_t* self);
void gameRenderHUD(engine_t *self);
void gameRenderScore(engine_t* self, int i);
void gameRenderHealth(engine_t* self, int i);
void gameRenderCoinCount(engine_t* self, int i);

// Update 
void gameUpdateBackgrounds(engine_t* self);
void gameUpdatePlayer(engine_t* self);
void gameUpdateEnemies(engine_t* self);
void gameUpdateProjectiles(engine_t* self);
void gameUpdateVfx(engine_t* self);
void gameUpdateTfx(engine_t* self);
void gameUpdateCollectables(engine_t *self);

// Game Logic Functions
void gameFireDefaultProjectile(engine_t* self);
void gameExplodePlayer(engine_t* self);
void gameExplodeEnemy(engine_t* self, enemy_t* enemy);
void gameSpawnEnemy(engine_t* self);
void gameLinearProjectileUpdate(projectile_t* self, void* _engine);
void gameSaucerFireProjectileFunction(enemy_t* enemy, void* _engine);
void gameIcyFireProjectileFunction(enemy_t* enemy, void* _engine);
void gameBlobFireProjectileFunction(enemy_t* enemy, void* _engine);
void gameRockFireProjectileFunction(enemy_t* enemy, void* _engine);

#endif // GAME_H
