#pragma once

#include <string>
#include <unordered_map>

#include "glm\glm.hpp"
struct ShaderProgramSource {
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};
class Shader {
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	ShaderProgramSource ParseShaderSourceFile(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);

	int GetUniformLocation(const std::string& name);

public:
	Shader(const std::string& filepath);
	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float x, float y, float z);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

};
