#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendereID)); // 获得一个Buffer对象的名称
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendereID)); // 顶点缓冲 将这个Buffer对象（名称）绑定到一个GL_ARRAY_BUFFER上， GL_ARRAY_BUFFER 是一个预定义的常量，表示缓冲对象的用途是顶点数组数据。
    //GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}
VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendereID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendereID));
}
void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
