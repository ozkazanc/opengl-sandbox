#pragma once

#include <string>

struct ShaderProgramSource {
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};
class Shader {
private:
	unsigned int m_ShaderID;

	ShaderProgramSource ParseShaderSourceFile(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);

public:
	Shader(const std::string& filepath);
	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Shader();

	void Use();
	void SetFloat();
};
