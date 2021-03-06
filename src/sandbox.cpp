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
#include "tests/Multiple3DModels.h"
#include "tests/ControlColorWithKeyboardTest.h"
#include "tests/MoveAround.h"

#include "tests/BasicLight.h"
#include "tests/LightingMap.h"

void GLFWErrorCallback(int error, const char* msg);
void GLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void GLFWKeyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
void ProcessInput(GLFWwindow* window, test::Test* &p_CurrentTest, test::TestMenu* p_TestMenu);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

const int g_WindowWidth = 640;
const int g_WindowHeight = 480;
static HandleInput *gp_InputHandler = new HandleInput();
bool gb_FirstMouse = true;
float gf_LastX = 320.0f;
float gf_LastY = 240.0f;
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
	//glfwSetKeyCallback(window, GLFWKeyCallbackWrapper);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

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
	p_TestMenu->RegisterTest<test::Multiple3DModels>("Multiple 3D Models");
	p_TestMenu->RegisterTest<test::MoveAround>("Move Camera");
	p_TestMenu->RegisterTest<test::BasicLight>("Basic Lighting");
	p_TestMenu->RegisterTest<test::LightingMap>("Texture Lighting");

	Renderer renderer;
	float fLastTime = 0.0f;
	float fCurrentTime = 0.0f;
	float fDeltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window, p_CurrentTest, p_TestMenu);
		/* Render here */
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
			
		fCurrentTime = (float)glfwGetTime();
		fDeltaTime = fCurrentTime - fLastTime;
		fLastTime = fCurrentTime;

		p_CurrentTest->OnUpdate(fDeltaTime);
		p_CurrentTest->OnRender();
		
		if (p_CurrentTest != p_TestMenu && ImGui::Button("<- Back to Menu (TAB)")){
			gp_InputHandler->RemoveObserver(p_CurrentTest);
			delete p_CurrentTest;
			p_CurrentTest = p_TestMenu;
		}
				
		p_CurrentTest->OnImGuiRender();
		p_CurrentTest->SetWindow(window);
		
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

static void ProcessInput(GLFWwindow* window, test::Test* &p_CurrentTest, test::TestMenu* p_TestMenu)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
		gp_InputHandler->GLFWKeyCallback(window, GLFW_KEY_ESCAPE, -1, GLFW_PRESS, -1);	
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
		gp_InputHandler->GLFWKeyCallback(window, GLFW_KEY_DOWN, -1, GLFW_PRESS, -1);	
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
		gp_InputHandler->GLFWKeyCallback(window, GLFW_KEY_UP, -1, GLFW_PRESS, -1);
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gp_InputHandler->GLFWKeyCallback(window, GLFW_KEY_W, -1, GLFW_PRESS, -1);
	
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gp_InputHandler->GLFWKeyCallback(window, GLFW_KEY_S, -1, GLFW_PRESS, -1);
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		gp_InputHandler->GLFWKeyCallback(window, GLFW_KEY_A, -1, GLFW_PRESS, -1);
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		gp_InputHandler->GLFWKeyCallback(window, GLFW_KEY_D, -1, GLFW_PRESS, -1);

	if (p_CurrentTest != p_TestMenu && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		gp_InputHandler->RemoveObserver(p_CurrentTest);
		delete p_CurrentTest;
		p_CurrentTest = p_TestMenu;
	}
	
}
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	float xPos_ = (float)xPos;
	float yPos_ = (float)yPos;
	if (gb_FirstMouse)
	{
		gf_LastX = xPos_;
		gf_LastY = yPos_;
		gb_FirstMouse = false;
	}

	float xOffset = xPos_ - gf_LastX;
	float yOffset = gf_LastY - yPos_; // reversed since y-coordinates go from bottom to top

	gf_LastX = xPos_;
	gf_LastY = yPos_;

	gp_InputHandler->MouseCallback(xOffset, yOffset, false);
}
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	gp_InputHandler->ScrollCallback((float)xoffset, (float)yoffset);
}
