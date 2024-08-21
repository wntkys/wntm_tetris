#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <GL/gl3w.h>
#include <inttypes.h>

typedef struct
{
    GLuint VBO, EBO, VAO;
} Drawable;

Drawable loadDrawable(float *verts, uint32_t verts_len, uint32_t *indices, uint32_t indices_len);

void freeDrawable(Drawable d);

#endif // DRAWABLE_H
