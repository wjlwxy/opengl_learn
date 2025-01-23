#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized) : m_type(type), m_count(count), m_normalized(normalized) {}
	unsigned int m_type;
	unsigned int m_count;
	unsigned char m_normalized; // GL_FALSE��GL_TRUE����unsigned char���� 

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4; 
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		:m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		//static_assert(false); // ���������һ�����������Ϊ������false������ζ�ţ������и�������ػ��汾�������κ����͵�T���ᵼ�´���
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count; // stride����Ҳ��Ҫ����count
	}
	// ����Ϊfloat�����ṩ�˾����ʵ�֡�����һ������GL_FLOAT��count��GL_FALSE��Ԫ����ӵ�m_Elements�����С�

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}
	// ����Ϊunsigned int�����ṩ�˾����ʵ�֡�����һ������GL_FLOAT��count��GL_FALSE��Ԫ����ӵ�m_Elements�����С�

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	// ����Ϊunsigned char�����ṩ�˾����ʵ�֡�����һ������GL_FLOAT��count��GL_FALSE��Ԫ����ӵ�m_Elements�����С�

	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	unsigned int GetStride() const { return m_Stride; }
};