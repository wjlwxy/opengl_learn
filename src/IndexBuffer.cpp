#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned* data, unsigned int count) : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendereID)); // 获得一个Buffer对象的名称
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendereID)); // 顶点缓冲 将这个Buffer对象（名称）绑定到一个Buffer上
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendereID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendereID));
}
void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
