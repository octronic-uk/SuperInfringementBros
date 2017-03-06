#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

#include "engine.h"

int highScoreSetupHandler  (engine_t* self);
int highScoreInputHandler  (engine_t* self);
int highScoreUpdateHandler (engine_t* self);
int highScoreRenderHandler (engine_t* self);
int highScoreCleanupHandler(engine_t* self);

#endif // HIGH_SCORE_H


