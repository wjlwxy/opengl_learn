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
	unsigned int id = glCreateShader(type); // 创建着色器
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); // 为着色器附加源代码
	glCompileShader(id); // 编译着色器

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char)); // alloca是C语言中用于在栈上开辟空间用的  使用堆区malloc也行，但是得注意释放资源
		/*const int len = length;
		char message[len];  即使使用const也不能初始化数组 */
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
	unsigned int program = glCreateProgram(); // 创建程序
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs); // 附加着色器到程序上
	glAttachShader(program, fs);
	glLinkProgram(program); // 链接程序
	glValidateProgram(program); // 验证程序

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

	return { ss[0].str(), ss[1].str() }; // 转成字符串再以结构体的形式返回，将自动构造结构体
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
	GLCall(glUniform1f(GetUniformLocation(name), value)); // 用于设置着色器中的浮点型uniform变量的值。
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