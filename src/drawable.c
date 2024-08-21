#include "drawable.h"

Drawable loadDrawable(float *verts, uint32_t verts_len, uint32_t *indices, uint32_t indices_len)
{
    Drawable d;
    glGenVertexArrays(1, &d.VAO);

    glGenBuffers(1, &d.VBO);
    glGenBuffers(1, &d.EBO);

    glBindVertexArray(d.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    glBufferData(GL_ARRAY_BUFFER, verts_len * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_len * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Unbind VAO
    glBindVertexArray(0);
    return d;
}

void freeDrawable(Drawable d)
{
    glDeleteVertexArrays(1, &d.VAO);
    glDeleteBuffers(1, &d.VBO);
    glDeleteBuffers(1, &d.EBO);
}