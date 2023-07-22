#pragma once

#include <string>
#include <iostream>

#include <glad/glad.h>

GLenum CheckGLError(const char* filename, int line);
void APIENTRY CheckGLDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

#define glCheckError() CheckGLError(__FILE__, __LINE__)