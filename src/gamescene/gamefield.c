#include "gamefield.h"

float easeOutBounce(float x);

float getLeft(float ss)
{
    return -ss * (GAME_FIELD_WIDTH - 1);
}

float getBottom(float ss)
{
    return -ss * (GAME_FIELD_HEIGHT - 1);
}

void GameField_LocalShapeSpace2FieldSpace_project(GameField const *field, FallingShape *shape, int rot, int y, int x, int *pY, int *pX)
{
    y = 3 - y;
    if (!shape->isRotationPointDiagonal)
    {
        int pivotY = 3 - (shape->rotationPoint[0]);
        int pivotX = (shape->rotationPoint[1]);

        switch (rot)
        {
        case 0:
            *pY = (0 + y) + shape->actualPosition[1];
            *pX = (0 + x) + shape->actualPosition[0];
            break;
        case 1:
            *pY = (0 + x) + shape->actualPosition[1] + (pivotY - pivotX);
            *pX = (3 - y) + shape->actualPosition[0] + (pivotX + pivotY - 3);
            break;
        case 2:
            *pY = (3 - y) + shape->actualPosition[1] + (-3 + pivotY + pivotY);
            *pX = (3 - x) + shape->actualPosition[0] + (-3 + pivotX + pivotX);
            break;
        case 3:
            *pY = (3 - x) + shape->actualPosition[1] + (-3 + pivotY + pivotX);
            *pX = (0 + y) + shape->actualPosition[0] + (-pivotY + pivotX);
            break;
        }
    }

    else if (shape->isRotationPointDiagonal)
    {

        switch (rot)
        {
        case 0:
            *pX = (0 + x) + shape->actualPosition[0];
            *pY = (0 + y) + shape->actualPosition[1];
            break;
        case 1:
            *pX = (3 - y) + shape->actualPosition[0];
            *pY = (0 + x) + shape->actualPosition[1];
            break;
        case 2:
            *pX = (3 - x) + shape->actualPosition[0];
            *pY = (3 - y) + shape->actualPosition[1];
            break;
        case 3:
            *pX = (0 + y) + shape->actualPosition[0];
            *pY = (3 - x) + shape->actualPosition[1];
            break;
        }
    }
    else
    {
        printf("wtf\n");
    }
}

void GameField_drawToConsole(GameField *field)
{

    for (int i = GAME_FIELD_HEIGHT - 1; i >= 0; i--)
    {
        for (int j = 0; j < GAME_FIELD_WIDTH; j++)
        {
            printf("%c ", field->grid[i][j] == 1 ? '#' : field->grid[i][j] == DEBUG_PLACEMENT_GHOST ? '?'
                                                                                                    : '.');
        }
        printf("\n");
    }
    printf("\n");
}

bool GameField_TryPlace(GameField *field, FallingShape *shape)
{

    bool hit = false;

    int myRot = shape->actualRotation % 4;
    if (myRot < 0)
    {
        myRot += 4;
    }

    for (int mY = 0; mY < 4; mY++)
    {
        for (int mX = 0; mX < 4; mX++)
        {
            if (shape->grid[mY][mX])
            {
                int x, y;

                GameField_LocalShapeSpace2FieldSpace_project(field, shape, myRot, mY, mX, &y, &x);

                // printf("%d %d\n", x, y);

                if (y < 0 || x < 0 || x >= GAME_FIELD_WIDTH)
                {
                    hit = true;
                }
                else if (y >= GAME_FIELD_HEIGHT)
                {
                    continue;
                }
                else if (field->grid[y][x] == 1)
                {
                    hit = true;
                }
                else
                {
                    field->grid[y][x] = DEBUG_PLACEMENT_GHOST;
                }
            }
        }
        // printf("\n");
    }

    return hit;
}

void GameField_Place(GameField *field, FallingShape *shape)
{
    int myRot = shape->actualRotation % 4;
    if (myRot < 0)
    {
        myRot += 4;
    }

    for (int mY = 0; mY < 4; mY++)
    {
        for (int mX = 0; mX < 4; mX++)
        {
            if (shape->grid[mY][mX])
            {
                int x, y;

                GameField_LocalShapeSpace2FieldSpace_project(field, shape, myRot, mY, mX, &y, &x);

                bool outOfBounds = (y > GAME_FIELD_HEIGHT || y < 0 || x < 0 || x >= GAME_FIELD_WIDTH);
                if (outOfBounds)
                {
                    puts("AAAA OoB");
                    continue;
                }
                field->grid[y][x] = 1;
                field->hues[y][x] = shape->hue;
            }
        }
    }
}

void GameField_Clear(GameField *field)
{
    for (int i = 0; i < GAME_FIELD_HEIGHT; i++)
    {
        for (int j = 0; j < GAME_FIELD_WIDTH; j++)
        {
            if (field->grid[i][j] == DEBUG_PLACEMENT_GHOST)
            {
                field->grid[i][j] = 0;
            }
        }
    }
}

void GameField_updateVisuals(GameField *field, float deltaTime)
{
    for (int i = 0; i < GAME_FIELD_HEIGHT; i++)
    {
        field->animation_states[i] -= deltaTime * 2;
        if (field->animation_states[i] < 0)
        {
            field->animation_states[i] = 0;
            field->animation_travel[i] = 0;
        }
    }
}

void GameField_draw(GameField const *field, float ss, GLuint shader, Drawable d, float aspectRatio, GLuint dbgShader)
{
    // GameField_drawToConsole(field);

    glUseProgram(shader);
    glUniform1f(glGetUniformLocation(shader, "aspectRatio"), aspectRatio);
    glUniform1f(glGetUniformLocation(shader, "scale"), ss);
    glUniform2f(glGetUniformLocation(shader, "position"), 0, 0);
    glUniform1f(glGetUniformLocation(shader, "rotation"), 0);
    for (int mY = 0; mY < GAME_FIELD_HEIGHT; mY++)
    {
        for (int mX = 0; mX < GAME_FIELD_WIDTH; mX++)
        {
            if (field->grid[mY][mX] == 1)
            {
                glUniform1f(glGetUniformLocation(shader, "hue_shift"), field->hues[mY][mX]);
                glUniform2f(glGetUniformLocation(shader, "localPos"),
                            mX * ss * 2 - ss * (GAME_FIELD_WIDTH - 1),
                            mY * ss * 2 - ss * (GAME_FIELD_HEIGHT - 1) + (ss * 2) * (field->animation_travel[mY] * (1 - easeOutBounce(1 - field->animation_states[mY]))));
                glBindVertexArray(d.VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
    }
}

void GameField_check(GameField *field)
{
    // tetris check if any rows are full
    for (int myY = GAME_FIELD_HEIGHT - 1; myY >= 0; myY--)
    {
        bool isFull = true;
        for (int myX = 0; myX < GAME_FIELD_WIDTH; myX++)
        {
            if (field->grid[myY][myX] != 1)
            {
                isFull = false;
                break;
            }
        }
        if (isFull)
        {
            // for animation
            for (int myY2 = myY; myY2 < GAME_FIELD_HEIGHT; myY2++)
            {
                field->animation_travel[myY2]++;
                field->animation_states[myY2] = 1;
            }

            printf("row %d is full\n", myY);
            for (int myX = 0; myX < GAME_FIELD_WIDTH; myX++)
            {
                field->grid[myY][myX] = 0;
            }

            // and now we need to shift everything down

            for (int myY2 = myY; myY2 < GAME_FIELD_HEIGHT - 1; myY2++)
            {
                field->animation_states[myY2] = field->animation_states[myY2 + 1];
                field->animation_travel[myY2] = field->animation_travel[myY2 + 1];

                for (int myX2 = 0; myX2 < GAME_FIELD_WIDTH; myX2++)
                {
                    field->grid[myY2][myX2] = field->grid[myY2 + 1][myX2];
                    field->hues[myY2][myX2] = field->hues[myY2 + 1][myX2];
                }
            }
        }
    }
}

// function from https://easings.net/#easeOutBounce

float easeOutBounce(float x)
{
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (x < 1 / d1)
    {
        return n1 * x * x;
    }
    else if (x < 2 / d1)
    {
        return n1 * (x -= 1.5 / d1) * x + 0.75;
    }
    else if (x < 2.5 / d1)
    {
        return n1 * (x -= 2.25 / d1) * x + 0.9375;
    }
    else
    {
        return n1 * (x -= 2.625 / d1) * x + 0.984375;
    }
}