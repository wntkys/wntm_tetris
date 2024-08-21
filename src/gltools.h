#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <GL/gl3w.h>

GLuint loadTexture(const char *path);

GLuint loadShader(GLenum type, const char *source);

GLuint loadProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint loadShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource);

#endif // GLTOOLS_H