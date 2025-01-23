#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized) : m_type(type), m_count(count), m_normalized(normalized) {}
	unsigned int m_type;
	unsigned int m_count;
	unsigned char m_normalized; // GL_FALSE和GL_TRUE都是unsigned char类型 

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
		//static_assert(false); // 这里会生成一个编译错误，因为条件是false。这意味着，除非有更具体的特化版本，否则任何类型的T都会导致错误。
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count; // stride可能也需要乘以count
	}
	// 这里为float类型提供了具体的实现。它将一个包含GL_FLOAT、count和GL_FALSE的元素添加到m_Elements容器中。

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}
	// 这里为unsigned int类型提供了具体的实现。它将一个包含GL_FLOAT、count和GL_FALSE的元素添加到m_Elements容器中。

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	// 这里为unsigned char类型提供了具体的实现。它将一个包含GL_FLOAT、count和GL_FALSE的元素添加到m_Elements容器中。

	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	unsigned int GetStride() const { return m_Stride; }
};