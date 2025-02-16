#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const; // const 确保const对象调用时能成功，const的作用是防止成员变量被改变
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }
};