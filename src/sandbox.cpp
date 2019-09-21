#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Renderer.h"
#include "HandleInput.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "tests/Test.h"
#include "tests/ClearColorTest.h"
#include "tests/Texture2DTest.h"
#include "tests/Texture3DTest.h"
#include "tests/ControlColorWithKeyboardTest.h"

void GLFWErrorCallback(int error, const char* msg);
void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void GLFWKeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);

const int g_WindowWidth = 640;
const int g_WindowHeight = 480;
static HandleInput *gp_InputHandler = new HandleInput();
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
	glfwSetKeyCallback(window, GLFWKeyCallbackWrapper);
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	// Initialize glew after getting a valid OpengGL context
	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Set up imgui context
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	/* Loop until the user closes the window */
	test::Test* p_CurrentTest = nullptr;
	test::TestMenu* p_TestMenu = new test::TestMenu(p_CurrentTest, gp_InputHandler);
	p_CurrentTest = p_TestMenu;

	p_TestMenu->RegisterTest<test::ColorClearTest>("Clear Color");
	p_TestMenu->RegisterTest<test::Texture2DTest>("2D Texture");
	p_TestMenu->RegisterTest<test::ControlColorWithKeyboardTest>("Control Color with Keyboard");
	p_TestMenu->RegisterTest<test::Texture3DTest>("3D Rotating Cube");

	Renderer renderer;
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
			
		
		p_CurrentTest->OnUpdate(0.0f);
		p_CurrentTest->OnRender();
		
		if (p_CurrentTest != p_TestMenu && ImGui::Button("<- Back to Menu")){
			gp_InputHandler->RemoveObserver(p_CurrentTest);
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
	if (p_CurrentTest != p_CurrentTest) //to avoid double freeing
		delete p_TestMenu;
	delete gp_InputHandler;
	
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

static void GLFWKeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods){
	gp_InputHandler->GLFWKeyCallback(window, key, scancode, action, mods);
}

