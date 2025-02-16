#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	int m_Location;
	std::string m_FilePath;
	std::string uniform;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	void SetUniform1f(const std::string& name, float value);

	void SetUniform1i(const std::string& name, int value);

	void SetUniform1iv(const std::string& name, int count, int* value);

};