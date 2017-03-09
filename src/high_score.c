#include "high_score.h"
#include "constants.h"
#include "logger.h"

int highScoreSetupHandler(engine_t* self) {
    // Background
    self->highScoreBackground = backgroundAllocate("res/gfx/high_score_bg.jpg",self->renderer);
    self->highScoreBackground->position.x = 0;
    self->highScoreBackground->position.y = 0;
    self->highScoreBackground->velocity.x = 0;
    self->highScoreBackground->velocity.y = 0;
    // Start Prompt
    self->highScorePromptText = textAllocate("res/fonts/nineteen.ttf",DEFAULT_FONT_SIZE,50);
    return 0;
}

int highScoreUpdateHandler(engine_t* self) {
    if (self->startPressed) {
        engineSetState(self,ENGINE_STATE_GAME);
    }
    return 0;
}

int highScoreRenderHandler(engine_t* self) { 
    SDL_Rect dest, *src;
    // Background
    background_t* bg = self->highScoreBackground;
    dest.w = bg->dimensions.x;
    dest.h = bg->dimensions.y;
    dest.x = bg->position.x;
    dest.y = bg->position.y;
    SDL_RenderCopy(self->renderer, bg->texture, NULL, &dest);
    // Propt Text
    SDL_Color black;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    black.a = 0;
    // Prompt Text Stroke
    TTF_SetFontOutline(self->highScorePromptText->font, 4);
    snprintf(self->highScorePromptText->text, self->highScorePromptText->bufferSize, "Press START to Play!");
    SDL_Surface *surface = TTF_RenderText_Solid(self->highScorePromptText->font, self->highScorePromptText->text, black);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(self->renderer, surface);
    if (texture == NULL){
        error("Error: Could not create coins text texture\n");
        return 0;
    }
    int iW, iH, cX, cY;
    SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
    dest.x = (self->screenWidth/2)-(iW/2);
    dest.y = self->screenHeight-iH-5;
    dest.w = iW;
    dest.h = iH;
    cX = dest.x + iW/2;
    cY = dest.y + iH/2;
    SDL_RenderCopy(self->renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    // Text
    TTF_SetFontOutline(self->highScorePromptText->font, 0);
    surface = TTF_RenderText_Solid(self->highScorePromptText->font, self->highScorePromptText->text, self->highScorePromptText->colour);
    texture = SDL_CreateTextureFromSurface(self->renderer, surface);
    if (texture == NULL){
        error("Error: Could not create coins text texture\n");
        return 0;
    }
    SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
    dest.x = cX-(iW/2);
    dest.y = cY-(iH/2);
    dest.w = iW;
    dest.h = iH;
    SDL_RenderCopy(self->renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return 0;
}

int highScoreCleanupHandler(engine_t* self) {
    // Background
    if (self->highScoreBackground != NULL) {
        backgroundDestroy(self->highScoreBackground);
        self->highScoreBackground = NULL;
    }
    // Prompt Text
    if (self->highScorePromptText != NULL) {
        textDestroy(self->highScorePromptText);
        self->highScorePromptText = NULL;
    }
    return 0;
}

size_t highScoreReadFile(engine_t* self) {
    FILE* highScoreFile = fopen(HIGH_SCORE_FILE,"r");
    if (highScoreFile == NULL) {
        error("Unable to open high score file");
        return 0;
    }

    size_t bytesWritten = fread(self->highScores,sizeof(int),HIGH_SCORES_MAX,highScoreFile);

    if (highScoreFile != NULL) {
        fclose(highScoreFile);
    } else {
        return 0;
    }
    
    return bytesWritten;
}

size_t highScoreWriteFile(engine_t* self) {
    FILE* highScoreFile = fopen(HIGH_SCORE_FILE,"w");
    if (highScoreFile == NULL) {
        error("Unable to open high score file");
        return 0;
    }

    size_t bytesWritten = fwrite(self->highScores,sizeof(int),HIGH_SCORES_MAX,highScoreFile);

    if (highScoreFile != NULL) {
        fclose(highScoreFile);
    } else {
        return 0; 
    }

    return bytesWritten;
}
