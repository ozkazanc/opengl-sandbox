#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "tests/Test.h"
#include "tests/ClearColorTest.h"

void GLFWErrorCallback(int error, const char* msg);
void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

const int g_WindowWidth = 640;
const int g_WindowHeight = 480;
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
	window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "Hello OpenGL", NULL, NULL);

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
			-40.0f, -40.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 40.0f, -40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 40.0f,  40.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-40.0f,  40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0,
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;

		VertexBuffer vb(vertices, 4 * 7 * sizeof(float));	//2 for position, 3 for color, 2 for texture coords
		VertexBufferLayout layout;
		layout.PushAttrib<float>(2);	//position attribute		
		layout.PushAttrib<float>(3);	//color attribute
		layout.PushAttrib<float>(2);	//texture coordinates
		
		va.AddBufferLayout(vb, layout);

		IndexBuffer ib(indices, 6);
		
		Shader shaderProgram("res/shaders/basic.shader");
		//Shader shaderProgram("res/shaders/simple.vs", "res/shaders/simple.fs");
				
		//Texture texture("res/textures/turkey-flag-icon-256.png");
		Texture texture("res/textures/mario-question-block-128.png");
		//Texture texture("res/textures/OpenGL_170px_June16.png");

		texture.Bind();
		shaderProgram.Bind();
		shaderProgram.SetUniform1i("u_TextureSlot", 0); // the slot id should be the same as the slot we bind our texture to
		
		// Unbind everything
		vb.Unbind();
		ib.Unbind();
		va.Unbind();
		shaderProgram.Unbind();

		Renderer renderer;

		// Set up imgui context
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");


		/* Loop until the user closes the window */
		glm::vec3 posA(80.0f, 100.0f, 0.0f);
		glm::vec3 posB(500.0f, 100.0f, 0.0f);
		glm::vec3 colorA(0.0f);
		glm::vec3 colorB(0.0f);


		test::Test* p_CurrentTest = nullptr;
		test::TestMenu* p_TestMenu = new test::TestMenu(p_CurrentTest);
		p_CurrentTest = p_TestMenu;

		p_TestMenu->RegisterTest<test::ColorClearTest>("Clear Color");

		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			

			//glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);				//640x480 pixel window, ortho proj
			//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));		//view(camera) matrix
			//
			//{
			//	//Render the first block
			//	glm::mat4 model = glm::translate(glm::mat4(1.0f), posA);
			//	glm::mat4 mvp = proj * view * model;
			//	
			//	shaderProgram.Bind();
			//	shaderProgram.SetUniform3f("u_Color", colorA.x, colorA.y, colorA.z);
			//	shaderProgram.SetUniformMat4f("u_MVP", mvp);
			//	renderer.Draw(va, ib, shaderProgram);
			//}
			//{
			//	//Render the second block
			//	glm::mat4 model = glm::translate(glm::mat4(1.0f), posB);
			//	glm::mat4 mvp = proj * view * model;

			//	shaderProgram.Bind();
			//	shaderProgram.SetUniform3f("u_Color", colorB.x, colorB.y, colorB.z);
			//	shaderProgram.SetUniformMat4f("u_MVP", mvp);
			//	renderer.Draw(va, ib, shaderProgram);
			//}
			//


			//ImGui::SliderFloat("XPositionA", &posA.x, 0.0f, 640.0f);
			//ImGui::SliderFloat("YPositionA", &posA.y, 0.0f, 480.0f);
			//ImGui::SliderFloat("XPositionB", &posB.x, 0.0f, 640.0f);
			//ImGui::SliderFloat("YPositionB", &posB.y, 0.0f, 480.0f);

			//ImGui::SliderFloat3("ColorA", glm::value_ptr(colorA), -1.0f, 1.0f);
			//ImGui::SliderFloat3("ColorB", glm::value_ptr(colorB), -1.0f, 1.0f);

			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			
			p_CurrentTest->OnUpdate(0.0f);
			p_CurrentTest->OnRender();
			if (p_CurrentTest != p_TestMenu && ImGui::Button("<- Back to Menu")){
				delete p_CurrentTest;
				p_CurrentTest = p_TestMenu;
			}
			
			
			p_CurrentTest->OnImGuiRender();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		delete p_CurrentTest;
		if (p_CurrentTest != p_CurrentTest)
			delete p_TestMenu;
	
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

