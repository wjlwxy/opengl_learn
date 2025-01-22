#include <iostream>

#include "Renderer.h"


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogError(const char* function, const char* file, const unsigned int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}