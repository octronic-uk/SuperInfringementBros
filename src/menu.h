#ifndef MENU_H
#define MENU_H

#include "engine.h"

// Menu Screen Functions
int menuSetupHandler  (engine_t* self);
int menuInputHandler  (engine_t* self);
int menuUpdateHandler (engine_t* self);
int menuRenderHandler (engine_t* self);
int menuCleanupHandler(engine_t* self);


#endif // MENU_H
