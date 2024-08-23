#pragma once

#include <GL/gl3w.h>
#include <cglm/cglm.h>
#include <SDL3/SDL.h>

typedef struct Scene_s
{
    void *data;
    int (*init)(struct Scene_s *);
    int (*update)(struct Scene_s *, uint64_t);
    int (*render)(struct Scene_s *);
    int (*destroy)(struct Scene_s *);
    int (*handleEvent)(struct Scene_s *, SDL_Event *event);

    int running;
    char next_scene[16];
    int window_width, window_height;
    float aspect_ratio;
} Scene;
