#include <stdio.h>
#include <stdlib.h>
#include <GL/gl3w.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <inttypes.h>
#include <cglm/cglm.h>
#include <math.h>

#include "drawable.h"
#include "fallingShape.h"
#include "gamefield.h"

#include "frag.h"
#include "vert.h"
#include "selDbg.h"

#include "gltools.h"

int main(int argc, char **argv)
{

	uint64_t sdl_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

	SDL_Window *window; // Declare a pointer

	SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL3 window", // window title
		640,			  // width, in pixels
		480,			  // height, in pixels
		sdl_window_flags  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL)
	{
		// In the case that the window could not be made...
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GLContext Context = SDL_GL_CreateContext(window);

	gl3wInit();

	float verts[] = {
		-1, -1,
		-1, 1,
		1, 1,
		1, -1};

	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0};

	Drawable d = loadDrawable(verts, 8, indices, 6);

	GLuint shader = loadShaderProgram(shader_vert, shader_frag);

	GLuint dbgShader = loadShaderProgram(shader_vert, selDbg_frag);

	GLuint texture = loadTexture("./box.png");

	printf("GL version: %s\n", glGetString(GL_VERSION));
	printf("texture: %d\n", texture);
	printf("shader: %d\n", shader);
	printf("VAO: %d\n", d.VAO);
	printf("VBO: %d\n", d.VBO);
	printf("EBO: %d\n", d.EBO);

	float aspectRatio = 1;
	bool Running = 1;

	FallingShape *shape = malloc(sizeof(FallingShape));
	shape->fallAccum = 0;
	shape->fallSpeed = 0.5;

	GameField *field = malloc(sizeof(GameField));
	memset(field->grid, 0, sizeof(uint8_t) * GAME_FIELD_WIDTH * GAME_FIELD_HEIGHT);

	FallingShape_generateNew(shape);

	// shape->actualPosition[0] = 0;
	shape->actualPosition[1] = 10;

	// shape->rotationPoint[0] = 3;
	// shape->rotationPoint[1] = 3;
	// uint8_t myShape[4][4] = {
	// 	{1, 0, 0, 0},
	// 	{1, 0, 0, 0},
	// 	{1, 0, 0, 0},
	// 	{1, 1, 1, 1}};

	// memcpy(shape->grid, myShape, sizeof(uint8_t) * 16);

	float oldTime = SDL_GetTicks() / 1000.0f;

	field->grid[0][0] = DEBUG_PLACEMENT_GHOST;
	while (Running)
	{
		SDL_Event Event;
		bool fallFaster = false;
		const uint8_t *keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_S])
		{
			fallFaster = true;
		}

		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_EVENT_KEY_DOWN)
			{
				switch (Event.key.key)
				{
				case SDLK_ESCAPE:
					Running = 0;
					break;

				default:
					break;
				}
				if (!GameField_TryPlace(field, shape))
				{
					switch (Event.key.scancode)
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
			else if (Event.type == SDL_EVENT_QUIT)
			{
				Running = 0;
			}
			else if (Event.type == SDL_EVENT_WINDOW_RESIZED)
			{

				SDL_GetWindowSizeInPixels(window, &Event.window.data1, &Event.window.data2);
				glViewport(0, 0, Event.window.data1, Event.window.data2);
				aspectRatio = (Event.window.data1 / (float)Event.window.data2);
			}
		}

		if (fallFaster)
		{
			shape->fallSpeed = 0.1;
		}
		else
		{
			shape->fallSpeed = 0.5;
		}

		float newTime = SDL_GetTicks() / 1000.0f;
		float deltaTime = newTime - oldTime;
		oldTime = newTime;

		glClearColor(0.3f, 0.3f, 0.3f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		float time = SDL_GetTicks() / 1000.0f;

		glUniform1f(glGetUniformLocation(shader, "aspectRatio"), aspectRatio);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		float ss = 1.f / 30;
		FallingShape_draw(shape, ss, shader, d, aspectRatio, getLeft(ss), getBottom(ss));
		FallingShape_UpdateVisuals(shape, deltaTime);
		bool fallen = FallingShape_fallUpdate(shape, deltaTime);

		bool hit = GameField_TryPlace(field, shape);

		if (fallen && hit)
		{
			FallingShape_fallBack(shape);
			GameField_Place(field, shape);
			shape->actualPosition[0] = 0;
			shape->actualPosition[1] = 10;
			FallingShape_generateNew(shape);

			FallingShape_UpdateVisuals(shape, 100);
			GameField_check(field);
		}
		GameField_draw(field, ss, shader, d, aspectRatio, dbgShader);
		GameField_Clear(field);

		SDL_GL_SwapWindow(window);
	}

	freeDrawable(d);
	return 0;
}