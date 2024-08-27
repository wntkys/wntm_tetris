#include "gamescene.h"

#include <SDL3/SDL.h>
#include "../stb_image.h"

#include "../drawable.h"
#include "../gltools.h"
#include "fallingShape.h"
#include "gamefield.h"

#include "field_shader.h"
#include <time.h>

typedef struct
{
    Drawable rect_drawable;
    GLuint rect_shader;
    GLuint rect_texture;

    GameField *field;
    FallingShape *shape;
} GameSceneData;

int init(Scene *self)
{
    srand(time(NULL));

    self->data = (GameSceneData *)malloc(sizeof(GameSceneData));
    GameSceneData *data = (GameSceneData *)self->data;

    float verts[] = {
        -1, -1,
        -1, 1,
        1, 1,
        1, -1};

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0};

    data->rect_drawable = loadDrawable(verts, 8, indices, 6);

    data->rect_shader = loadShaderProgram(field_shader_vs, field_shader_fs);

    data->rect_texture = loadTexture("./box.png");

    float aspectRatio = 1;
    bool Running = 1;

    data->shape = malloc(sizeof(FallingShape));
    data->shape->fallAccum = 0;
    data->shape->fallSpeed = 0.5;

    data->field = malloc(sizeof(GameField));
    memset(data->field->grid, 0, sizeof(uint8_t) * GAME_FIELD_WIDTH * GAME_FIELD_HEIGHT);

    FallingShape_generateNew(data->shape);

    data->shape->actualPosition[0] = 4;
    data->shape->actualPosition[1] = 20;
}

int destroy(Scene *self)
{

    GameSceneData *data = (GameSceneData *)self->data;
    freeDrawable(data->rect_drawable);
    glDeleteTextures(1, &data->rect_texture);
    glDeleteBuffers(1, &data->rect_drawable.VBO);
    glDeleteBuffers(1, &data->rect_drawable.EBO);
    glDeleteVertexArrays(1, &data->rect_drawable.VAO);
    free(data->field);
    free(data->shape);
    free(self->data);

    return 0;
}

int update(Scene *self, uint64_t delta)
{
    GameSceneData *data = (GameSceneData *)self->data;

    float deltaTime = delta / 1000.0f;

    FallingShape *shape = data->shape;
    GameField *field = data->field;

    GameField_Clear(field);

    FallingShape_UpdateVisuals(shape, deltaTime);

    GameField_updateVisuals(field, deltaTime);

    bool fallen = FallingShape_fallUpdate(shape, deltaTime);

    bool hit = GameField_TryPlace(field, shape);

    if (fallen && hit)
    {
        FallingShape_fallBack(shape);
        GameField_Place(field, shape);
        shape->actualPosition[0] = 4;
        shape->actualPosition[1] = 20;
        FallingShape_generateNew(shape);

        FallingShape_UpdateVisuals(shape, 100);
        GameField_check(field);
    }

    return 0;
}

int render(Scene *self)
{

    GameSceneData *data = (GameSceneData *)self->data;

    GLuint shader = data->rect_shader;
    float aspectRatio = self->aspect_ratio;
    GLuint texture = data->rect_texture;
    Drawable rect_drawable = data->rect_drawable;
    FallingShape *shape = data->shape;
    GameField *field = data->field;

    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader);

    glUniform1f(glGetUniformLocation(shader, "aspectRatio"), aspectRatio);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    float ss = 1.f / 30;
    FallingShape_draw(shape, ss, shader, rect_drawable, aspectRatio, getLeft(ss), getBottom(ss));

    GameField_draw(field, ss, shader, rect_drawable, aspectRatio, shader);

    return 0;
}

int handleEvent(Scene *self, SDL_Event *event)
{

    GameSceneData *data = (GameSceneData *)self->data;

    FallingShape *shape = data->shape;
    GameField *field = data->field;

    if (event->type == SDL_EVENT_KEY_DOWN)
    {

        if (!GameField_TryPlace(data->field, data->shape))
        {
            switch (event->key.scancode)
            {
            case SDL_SCANCODE_R:
                FallingShape_tryRotate(shape, 1);
                if (GameField_TryPlace(field, shape))
                {
                    FallingShape_tryMove(shape, 1);
                    if (GameField_TryPlace(field, shape))
                    {
                        FallingShape_tryMove(shape, 0);
                        FallingShape_tryMove(shape, 0);
                        if (GameField_TryPlace(field, shape))
                        {

                            FallingShape_tryMove(shape, 0);
                            if (GameField_TryPlace(field, shape))
                            {
                                FallingShape_tryMove(shape, 1);
                                FallingShape_tryMove(shape, 1);
                                FallingShape_tryMove(shape, 1);
                                FallingShape_tryMove(shape, 1);
                                if (GameField_TryPlace(field, shape))
                                {
                                    FallingShape_tryMove(shape, 0);
                                    FallingShape_tryMove(shape, 0);
                                    FallingShape_tryRotate(shape, 0);
                                }
                            }
                        }
                    }
                }
                break;

            case SDL_SCANCODE_L:
                FallingShape_tryRotate(shape, 0);
                if (GameField_TryPlace(field, shape))
                {
                    FallingShape_tryRotate(shape, 1);
                }
                break;
            case SDL_SCANCODE_A:
                FallingShape_tryMove(shape, 1);
                if (GameField_TryPlace(field, shape))
                {
                    FallingShape_tryMove(shape, 0);
                }
                break;

            case SDL_SCANCODE_D:
                FallingShape_tryMove(shape, 0);
                if (GameField_TryPlace(field, shape))
                {
                    FallingShape_tryMove(shape, 1);
                }
                break;
            default:
                break;
            }
        }
    }

    bool fallFaster = false;
    const uint8_t *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_S])
    {
        fallFaster = true;
    }

    if (fallFaster)
    {
        shape->fallSpeed = 0.1;
    }
    else
    {
        shape->fallSpeed = 0.5;
    }
    return 0;
}

void getGameScene(Scene *scene)
{
    scene->init = init;
    scene->update = update;
    scene->render = render;
    scene->destroy = destroy;
    scene->handleEvent = handleEvent;
}