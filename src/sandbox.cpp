#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLFWErrorCallback(int error, const char* msg);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);

void GLClearError();
bool GLLogCall(const char* funcName, const char* file, int line);

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(GLFWErrorCallback);
	
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, GLFWKeyCallback);
	glfwSetFramebufferSizeCallback(window, GLFWFramebufferSizeCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//have to call glewInit after having a valid OpenGl context
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glBegin(GL_TRIANGLES));
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0, 0.5f);
		glEnd();
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
static void GLClearError(){
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* funcName, const char* file, int line){
	while (unsigned int error = glGetError()){
		std::cout << "[OpenGL Error](" << error << ") - "<<
			"In function: " << funcName << ", line: " << line << ", in: " << file << std::endl;
		return false;
	}
	return true;
}
static void GLFWErrorCallback(int error, const char* msg){
	fprintf(stderr, "Error: %s\n", msg);
}
static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	//close the window if Escape is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
static void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}