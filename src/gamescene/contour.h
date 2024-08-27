#pragma once

#include "../drawable.h"

#include <cglm/cglm.h>
#include <GL/gl3w.h>
#include <math.h>
#include <stddef.h>

typedef Drawable ContourDrawable;

typedef struct
{
    vec2 pos;
    uint8_t flags;
} ContourVertex;

ContourDrawable
_loadContourDrawable(float const *verts, uint32_t verts_len, uint32_t const *indices, uint32_t indices_len);

void ContourDrawable_get(ContourDrawable *self);
