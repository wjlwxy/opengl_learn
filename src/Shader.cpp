#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(), m_Location(-1), m_RendererID(0), uniform("")
{
	GLCall(ShaderProgramSource source = ParseShader(filepath));
	//std::cout << "VERTEX" << std::endl;
	//std::cout << source.VertexSource << std::endl;
	//std::cout << "FRAGMENT" << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); // ������ɫ��
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); // Ϊ��ɫ������Դ����
	glCompileShader(id); // ������ɫ��

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // alloca��C������������ջ�Ͽ��ٿռ��õ�  ʹ�ö���mallocҲ�У����ǵ�ע���ͷ���Դ
		/*const int len = length;
		char message[len];  ��ʹʹ��constҲ���ܳ�ʼ������ */
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Fail to compile" << (type == GL_VERTEX_SHADER ? "Vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& VertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram(); // ��������
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs); // ������ɫ����������
	glAttachShader(program, fs);
	glLinkProgram(program); // ���ӳ���
	glValidateProgram(program); // ��֤����

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("Vertex") != std::string::npos)
				type = ShaderType::VERTEX;//set mode to Vertex
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;// set mode to fragment
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() }; // ת���ַ������Խṹ�����ʽ���أ����Զ�����ṹ��
}



void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	
	 GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	 uniform = name;

		
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value)); // ����������ɫ���еĸ�����uniform������ֵ��
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		if (m_UniformLocationCache[name] == -1)
			std::cout << "Warning: uniform '" << name << "' doesn't exist! or doesn't be used!" << std::endl;
		return m_UniformLocationCache[name];
	}
	GLCall(m_Location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (m_Location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist! or doesn't be used!" << std::endl;

	m_UniformLocationCache[name] = m_Location;
	return m_Location;
}