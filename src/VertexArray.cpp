#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RenderedID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RenderedID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		/* 设置顶点缓冲布局， 会绑定当前的一个顶点缓冲区 ，同时也会将顶点缓冲区与顶点数组对象绑定，使得后面只需要绑定顶点数组对象就行, 虽然position被设置为vec4，但是这里保有全部positions_texcoords数组数据，然后根据下方m_count和stride确定取哪些数据（position 2个，texcoord 2个)*/
		GLCall(glVertexAttribPointer(i, element.m_count, element.m_type, element.m_normalized, layout.GetStride(), (const void*)offset)); // 第一个0为顶点属性中位置属性的索引，在着色器中被指定   参数2为一个属性所占元素数量， 2 * sizeof(float)为一整组属性所占字节数， 最后的offset是此属性的起始偏移
		offset += element.m_count * VertexBufferElement::GetSizeOfType(element.m_type);
	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RenderedID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}