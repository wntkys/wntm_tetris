#include <stdio.h>
#include <stdlib.h>
#include <GL/gl3w.h>
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <inttypes.h>
#include <cglm/cglm.h>
#include <math.h>

#include "gamescene/gamescene.h"

int main(int argc, char **argv)
{

	uint64_t sdl_window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;

	SDL_Window *window;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("wntm:tetris", 640, 480, sdl_window_flags);

	if (window == NULL)
	{

		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GLContext Context = SDL_GL_CreateContext(window);
	gl3wInit();

	Scene *scene = malloc(sizeof(Scene));
	getGameScene(scene);

	scene->init(scene);

	// GLuint dbgShader = loadShaderProgram(shader_vert, selDbg_frag);

	uint64_t oldTime = SDL_GetTicks();

	bool running = true;

	while (running)
	{
		SDL_Event Event;

		while (SDL_PollEvent(&Event))
		{

			if (Event.type == SDL_EVENT_QUIT)
			{
				running = 0;
			}
			else if (Event.type == SDL_EVENT_WINDOW_RESIZED)
			{

				SDL_GetWindowSizeInPixels(window, &Event.window.data1, &Event.window.data2);
				glViewport(0, 0, Event.window.data1, Event.window.data2);
				scene->window_width = Event.window.data1;
				scene->window_height = Event.window.data2;
				scene->aspect_ratio = (Event.window.data1 / (float)Event.window.data2);
			}
			scene->handleEvent(scene, &Event);
		}

		uint64_t newTime = SDL_GetTicks();
		uint64_t deltaTime = newTime - oldTime;
		oldTime = newTime;

		scene->update(scene, deltaTime);
		scene->render(scene);

		SDL_GL_SwapWindow(window);
	}

	return 0;
}