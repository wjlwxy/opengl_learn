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
		/* ���ö��㻺�岼�֣� ��󶨵�ǰ��һ�����㻺���� ��ͬʱҲ�Ὣ���㻺�����붥���������󶨣�ʹ�ú���ֻ��Ҫ�󶨶�������������, ��Ȼposition������Ϊvec4���������ﱣ��ȫ��positions_texcoords�������ݣ�Ȼ������·�m_count��strideȷ��ȡ��Щ���ݣ�position 2����texcoord 2��)*/
		GLCall(glVertexAttribPointer(i, element.m_count, element.m_type, element.m_normalized, layout.GetStride(), (const void*)offset)); // ��һ��0Ϊ����������λ�����Ե�����������ɫ���б�ָ��   ����2Ϊһ��������ռԪ�������� 2 * sizeof(float)Ϊһ����������ռ�ֽ����� ����offset�Ǵ����Ե���ʼƫ��
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