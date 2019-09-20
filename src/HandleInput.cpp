#include "HandleInput.h"

void HandleInput::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Close the window is Escape key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
		Notify(GLFW_KEY_ESCAPE);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		Notify(GLFW_KEY_DOWN);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS){
		Notify(GLFW_KEY_UP);
	}
}