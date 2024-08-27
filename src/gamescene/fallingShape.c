#include "fallingShape.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#define max(a, b) (((a) > (b)) ? (a) : (b))

void FallingShape_tryRotate(FallingShape *shape, bool clockwise)
{
    shape->actualRotation = clockwise ? shape->actualRotation + 1 : shape->actualRotation - 1;
}

void FallingShape_generateNew(FallingShape *shape)
{
    int a = rand() % 5;
    shape->actualRotation = 0;
    shape->hue = (rand() / (float)RAND_MAX) * 2 * M_PI;

    if (a == 0)
    {
        uint8_t myShape[4][4] = {
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}};
        shape->rotationPoint[0] = 1;
        shape->rotationPoint[1] = 0;
        shape->isRotationPointDiagonal = false;
        memcpy(shape->grid, myShape, sizeof(uint8_t) * 16);
    }
    if (a == 1)
    {
        uint8_t myShape[4][4] = {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}};
        shape->rotationPoint[0] = 1;
        shape->rotationPoint[1] = 1;
        shape->isRotationPointDiagonal = false;

        memcpy(shape->grid, myShape, sizeof(uint8_t) * 16);
    }
    if (a == 2)
    {
        uint8_t myShape[4][4] = {
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0}};
        shape->rotationPoint[0] = 2;
        shape->rotationPoint[1] = 0;
        shape->isRotationPointDiagonal = false;

        memcpy(shape->grid, myShape, sizeof(uint8_t) * 16);
    }
    if (a == 3)
    {
        uint8_t myShape[4][4] = {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}};
        shape->rotationPoint[0] = 1;
        shape->rotationPoint[1] = 1;
        shape->isRotationPointDiagonal = true;

        memcpy(shape->grid, myShape, sizeof(uint8_t) * 16);
    }
    if (a == 4)
    {
        uint8_t myShape[4][4] = {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}};
        shape->rotationPoint[0] = 1;
        shape->rotationPoint[1] = 0;
        shape->isRotationPointDiagonal = false;

        memcpy(shape->grid, myShape, sizeof(uint8_t) * 16);
    }
}
void FallingShape_UpdateVisuals(FallingShape *shape, float delta)
{
    {
        float deltaRot = fabsf(shape->actualRotation - shape->rotation);
        float speed = max(deltaRot / 0.1, 15);
        if (deltaRot > delta * speed)
        {

            int sign = shape->actualRotation > shape->rotation ? 1 : -1;
            shape->rotation = shape->rotation + sign * delta * speed;
        }
        else
        {
            shape->rotation = shape->actualRotation;
        }
    }
    {
        float deltaPosx = fabsf(shape->actualPosition[0] - shape->position[0]);
        float speed = max(deltaPosx / 0.03, 15);
        if (deltaPosx > delta * speed)
        {
            int sign = shape->actualPosition[0] > shape->position[0] ? 1 : -1;
            shape->position[0] = shape->position[0] + sign * delta * speed;
        }
        else
        {
            shape->position[0] = shape->actualPosition[0];
        }
    }
    {
        float deltaPosy = fabsf(shape->actualPosition[1] - shape->position[1]);
        float speed = max(deltaPosy / 0.03, 15);
        if (deltaPosy > delta * speed)
        {

            int sign = shape->actualPosition[1] > shape->position[1] ? 1 : -1;
            shape->position[1] = shape->position[1] + sign * delta * speed;
        }
        else
        {
            shape->position[1] = shape->actualPosition[1];
        }
    }
}

void FallingShape_draw(const FallingShape *shape, float ss, GLuint shader, Drawable d, float aspectRatio, float originx, float originy)
{
    float time = 0;

    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "aspectRatio"), aspectRatio);
    glUniform1f(glGetUniformLocation(shader, "scale"), ss);

    vec2 delta = {(shape->rotationPoint[1] * 2 + shape->isRotationPointDiagonal) * ss,
                  (6 - (shape->rotationPoint[0] * 2 + shape->isRotationPointDiagonal)) * ss};

    glUniform2f(glGetUniformLocation(shader, "position"), (shape->position[0] * 2) * ss + originx + delta[0], (shape->position[1] * 2) * ss + originy + delta[1]);
    glUniform1f(glGetUniformLocation(shader, "rotation"), M_PI * 0.5f * shape->rotation);
    glUniform1f(glGetUniformLocation(shader, "hue_shift"), shape->hue);

    for (int mY = 0; mY < 4; mY++)
    {
        for (int mX = 0; mX < 4; mX++)
        {
            if (shape->grid[mY][mX])
            {

                glUniform2f(glGetUniformLocation(shader, "localPos"), mX * ss * 2 - delta[0], (3 - mY) * ss * 2 - delta[1]);
                glBindVertexArray(d.VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
    }
}

void FallingShape_tryMove(FallingShape *shape, bool left)
{
    shape->actualPosition[0] = left ? shape->actualPosition[0] - 1 : shape->actualPosition[0] + 1;
}

void FallingShape_fallBack(FallingShape *shape)
{
    shape->actualPosition[1]++;
}

bool FallingShape_fallUpdate(FallingShape *shape, float delta)
{
    bool fallen = false;
    shape->fallAccum += delta;
    if (shape->fallAccum > shape->fallSpeed)
    {

        shape->fallAccum -= shape->fallSpeed;
        fallen = true;
        shape->actualPosition[1]--;
    }
    return fallen;
}