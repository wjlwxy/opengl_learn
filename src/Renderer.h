#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// __debugbreak()��visual studio�ĺ���
#define ASSERT(x) if (!(x)) __debugbreak();
// #x ���԰Ѻ���xת��Ϊ�ַ���
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogError(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogError(const char* function, const char* file, const unsigned int line);