#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexArray;
class IndexBuffer;
class Shader;

// __debugbreak()��visual studio�ĺ���
#define ASSERT(x) if (!(x)) __debugbreak();
// #x ���԰Ѻ���xת��Ϊ�ַ���
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogError(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogError(const char* function, const char* file, const unsigned int line);

class Renderer
{
private:

public:
    void Clear() const;
    void SetClearColor(float red, float green, float blue, float alpha) const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};