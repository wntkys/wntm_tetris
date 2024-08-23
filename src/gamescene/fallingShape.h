#ifndef FALLINGSHAPE_H
#define FALLINGSHAPE_H

#include <inttypes.h>
#include <cglm/cglm.h>
#include <GL/gl3w.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../drawable.h"

typedef struct
{
    uint8_t grid[4][4];
    vec3 color;
    float rotation;
    vec2 position;

    float falseMoveReact;
    float hue;

    int rotationPoint[2];
    bool isRotationPointDiagonal;

    int actualPosition[2];
    char actualRotation;

    float fallAccum;
    float fallSpeed;

} FallingShape;

#include "gamefield.h"

void FallingShape_tryRotate(FallingShape *shape, bool clockwise);

void FallingShape_UpdateVisuals(FallingShape *shape, float delta);

void FallingShape_draw(const FallingShape *shape, float ss, GLuint shader, Drawable d, float aspectRatio, float originx, float originy);

bool FallingShape_fallUpdate(FallingShape *shape, float delta);

void FallingShape_fallBack(FallingShape *shape);

void FallingShape_tryMove(FallingShape *shape, bool left);

void FallingShape_generateNew(FallingShape *shape);
#endif // FALLINGSHAPE_H