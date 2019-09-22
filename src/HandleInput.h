#pragma once

#include "Observer.h"
#include "GLFW/glfw3.h"

class HandleInput : public Subject{
	public:
		void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void MouseCallback(float x, float y, bool scroll);
		void ScrollCallback(float x, float y);
};
