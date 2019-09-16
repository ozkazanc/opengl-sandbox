#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

void GLFWErrorCallback(int error, const char* msg);
void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
	
	// Initialize glew after getting a valid OpengGL context
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	// This scope is here to ensure all stack allocated class objects (vertex buffers, index buffers, etc.) are destroyed before glfwTerminate() is called
	{
		float vertices[] = {
			-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f
		};
		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0,
		};

		VertexArray va;

		VertexBuffer vb(vertices, 4 * 5 * sizeof(float));	//2 for position, 3 for color
		VertexBufferLayout layout;
		layout.PushAttrib<float>(2);	//position attribute		
		layout.PushAttrib<float>(3);	//color attribute
		
		va.AddBufferLayout(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shaderProgram("res/shaders/basic.shader");
		//Shader shaderProgram("res/shaders/simple.vs", "res/shaders/simple.fs");
				
		// Unbind everything
		vb.Unbind();
		ib.Unbind();
		va.Unbind();
		shaderProgram.Unbind();

		Renderer renderer;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			// Update the uniform color
			float T = 0.5;
			float timeValue = glfwGetTime();
			float redValue = sin(timeValue * T) / 2.0f + 0.5f;
			float greenValue = sin(timeValue * T) / 2.0f + 0.5f;
			float blueValue = cos(timeValue * T) / 2.0f + 0.5f;
	
			shaderProgram.Bind();
			shaderProgram.SetUniform4f("u_Color", redValue, greenValue, blueValue, 1.0f);
			
			renderer.Draw(va, ib, shaderProgram);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
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

