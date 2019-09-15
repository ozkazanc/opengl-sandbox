#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource {
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};

void GLFWErrorCallback(int error, const char* msg);
void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned int CompileShader(unsigned int type, const std::string& source);
unsigned int CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
ShaderProgramSource ParseShaderSourceFile(const std::string& filepath);

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(GLFWErrorCallback);
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, GLFWFramebufferSizeCallback);
	glfwSetKeyCallback(window, GLFWKeyCallback);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	// Initialize glew after the current context
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f,
		};
		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0,
		};

		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));
	
		VertexBuffer vbo(positions, 4 * 2 * sizeof(float));

		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		IndexBuffer ibo(indices, 6);

		// Unbind all buffers
		vbo.Unbind();
		ibo.Unbind();
		GLCall(glBindVertexArray(0));

		ShaderProgramSource src = ParseShaderSourceFile("res/shaders/basic.shader");
		unsigned int shaderProgram = CreateShaderProgram(src.vertexShaderSource, src.fragmentShaderSource);

		GLCall(int vertexColorLocation = glGetUniformLocation(shaderProgram, "u_Color"));
		ASSERT(vertexColorLocation != -1);
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			// update the uniform color
			float timeValue = glfwGetTime();
			float redValue = sin(timeValue) / 2.0f + 0.5f;
			float blueValue = cos(timeValue) / 2.0f + 0.5f;

			GLCall(glUseProgram(shaderProgram));
			GLCall(glUniform4f(vertexColorLocation, redValue, 0.0f, blueValue, 1.0f));

			GLCall(glBindVertexArray(vao));
			ibo.Bind();

			//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		GLCall(glDeleteProgram(shaderProgram));
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


static void GLFWErrorCallback(int error, const char *msg) {
	fprintf(stderr, "[Glfw Error]: %s\n", msg);
}

static void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Close the window is Escape key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
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

static unsigned int CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) {
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

static ShaderProgramSource ParseShaderSourceFile(const std::string& filepath) {
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

