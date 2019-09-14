#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLFWErrorCallback(int error, const char* msg);
void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLClearError();
bool GLLogCall(const char* funcName, const char* file, int line);
unsigned int CompileShader(unsigned int type, const std::string& source);
unsigned int CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(0.4f, 0.8f, 0.5f, 1.0f);\n"
"}\n\0";

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(GLFWErrorCallback);
	/* Initialize the library */
	if (!glfwInit())
		return -1;

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

	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};

	unsigned int vbo;
	GLCall(glGenBuffers(1, &vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER,vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	unsigned int shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
	GLCall(glUseProgram(shaderProgram));
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClearColor(0.9f, 0.5f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
		
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
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
static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* funcName, const char* file, int line) {
	while (unsigned int error = glGetError()) {
		std::cerr << "[OpenGL Error](" << error << ") - " << "Line:" << line << ", In function: " << funcName << ", in: " << file << std::endl;
		return false;
	}
	return true;
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

