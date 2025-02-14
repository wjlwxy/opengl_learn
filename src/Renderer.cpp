#include <iostream>

#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

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

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT)); // 清除颜色缓冲区。当你调用这个函数时，它会将颜色缓冲区的内容设置为当前清除颜色（clear color），这个清除颜色可以通过 glClearColor 函数来设置。通常在渲染每一帧之前使用，以确保上一帧的图像不会干扰当前帧的绘制。
}

void Renderer::SetClearColor(float red, float green, float blue, float alpha) const
{
    GLCall(glClearColor(red, green, blue, alpha));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    
    va.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
