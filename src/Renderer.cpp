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
    GLCall(glClear(GL_COLOR_BUFFER_BIT)); // �����ɫ����������������������ʱ�����Ὣ��ɫ����������������Ϊ��ǰ�����ɫ��clear color������������ɫ����ͨ�� glClearColor ���������á�ͨ������Ⱦÿһ֮֡ǰʹ�ã���ȷ����һ֡��ͼ�񲻻���ŵ�ǰ֡�Ļ��ơ�
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
