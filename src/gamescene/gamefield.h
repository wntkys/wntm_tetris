#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <GL/gl3w.h>

#include "../drawable.h"
#include "fallingShape.h"

#define GAME_FIELD_WIDTH 12
#define GAME_FIELD_HEIGHT 20

#define DEBUG_PLACEMENT_GHOST 15

typedef struct
{
    char grid[GAME_FIELD_HEIGHT][GAME_FIELD_WIDTH];
    float hues[GAME_FIELD_HEIGHT][GAME_FIELD_WIDTH];
} GameField;

float getLeft(float ss);

float getBottom(float ss);

bool GameField_TryPlace(GameField *field, FallingShape *shape);

void GameField_Place(GameField *field, FallingShape *shape);

void GameField_Clear(GameField *field);

void GameField_draw(GameField *field, float ss, GLuint shader, Drawable d, float aspectRatio, GLuint dbgShader);

void GameField_check(GameField *field);
#endif // GAMEFIELD_H