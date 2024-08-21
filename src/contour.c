#include "contour.h"

ContourDrawable _loadContourDrawable(float const *verts, uint32_t verts_len, uint32_t const *indices, uint32_t indices_len)
{
    ContourDrawable d;
    glGenVertexArrays(1, &d.VAO);

    glGenBuffers(1, &d.VBO);
    glGenBuffers(1, &d.EBO);

    glBindVertexArray(d.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
    glBufferData(GL_ARRAY_BUFFER, verts_len * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_len * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ContourVertex), (void *)offsetof(ContourVertex, pos));

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, sizeof(ContourVertex), (void *)offsetof(ContourVertex, flags));
    // Unbind VAO
    glBindVertexArray(0);
    return d;
}

void ContourDrawable_get(ContourDrawable *self)
{
    const ContourVertex vertices[] = {
        // lefttop corner(void*)(void*)p
        {{0.0, 0.0}, 0b00000000},
        {{0.0, 0.5}, 0b00000000},
        {{0.5, 0.5}, 0b00000000},
        {{0.5, 0.0}, 0b00000000},

        // righttop corner
        {{0.5, 0.0}, 0b00000001},
        {{0.5, 0.5}, 0b00000001},
        {{1.0, 0.5}, 0b00000001},
        {{1.0, 0.0}, 0b00000001},

        // rightbottom corner
        {{1.0, 0.0}, 0b00000011},
        {{1.0, 0.5}, 0b00000011},
        {{0.5, 0.5}, 0b00000011},
        {{0.5, 1.0}, 0b00000011},

        // leftbottom corner
        {{0.5, 1.0}, 0b00000010},
        {{0.5, 0.5}, 0b00000010},
        {{0.0, 0.5}, 0b00000010},
        {{0.0, 1.0}, 0b00000010},
    };

    uint32_t indices[] = {
        1, 0, 2,
        2, 0, 3,

        2, 3, 4,
        2, 4, 5,

        5, 6, 8,
        6, 11, 8,

        8, 9, 10,
        8, 10, 11,

        8, 9, 15,
        15, 9, 14,

        12, 15, 14,
        12, 14, 14,

        12, 15, 2,
        12, 2, 1};

    *self = _loadContourDrawable((float *)vertices, sizeof(vertices) / sizeof(ContourVertex), indices, sizeof(indices) / sizeof(uint32_t));
}