#ifndef TITLE_H
#define TITLE_H

#include "engine.h"

int titleSetupHandler(engine_t* self);
int titleInputHandler(engine_t* self);
int titleUpdateHandler(engine_t* self);
int titleRenderHandler(engine_t* self);
int titleCleanupHandler(engine_t* self);

#endif // TITLE_H
