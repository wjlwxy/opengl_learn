#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendereID)); // ���һ��Buffer���������
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendereID)); // ���㻺�� �����Buffer�������ƣ��󶨵�һ��GL_ARRAY_BUFFER�ϣ� GL_ARRAY_BUFFER ��һ��Ԥ����ĳ�������ʾ����������;�Ƕ����������ݡ�
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
