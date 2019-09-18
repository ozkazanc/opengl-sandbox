#include "Shader.h"

#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath) 
	:m_Filepath(filepath), 
	 m_RendererID(0)
{
	ShaderProgramSource src = ParseShaderSourceFile(filepath);
	m_RendererID = CreateShaderProgram(src.vertexShaderSource, src.fragmentShaderSource);
}

Shader::Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath) 
	:m_Filepath(vertexFilepath + " & " + fragmentFilepath),
	 m_RendererID(0)
{
	ShaderProgramSource first = ParseShaderSourceFile(vertexFilepath);
	ShaderProgramSource second = ParseShaderSourceFile(fragmentFilepath);

	m_RendererID = CreateShaderProgram(first.vertexShaderSource, second.fragmentShaderSource);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w) {
	GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix){
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) {
	// Search the cache for the uniform location first
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform: '" << name << "' doesn't exist!" << std::endl;
	
	// Add uniform name to hash table to cache it
	m_UniformLocationCache[name] = location;
	
	return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	GLCall(unsigned int shader = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(shader, 1, &src, nullptr));
	GLCall(glCompileShader(shader));

	int success;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (success != GL_TRUE) {

		int length;
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(shader, length, nullptr, message));

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(shader));
		return 0;
	}
	return shader;
}

unsigned int Shader::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) {
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	int success;
	char infoLog[512];
	GLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));
	if (!success) {
		GLCall(glGetProgramInfoLog(program, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

ShaderProgramSource Shader::ParseShaderSourceFile(const std::string& filepath) {
	std::ifstream stream(filepath);
	if (stream.fail()) std::cout << "Error: File path cannot be found!" << std::endl;

	enum ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}