#include "Renderer.h"

#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* funcName, const char* file, int line) {
	while (unsigned int error = glGetError()) {
		std::cerr << "[OpenGL Error](" << error << ") - " << "Line:" << line << ", In function: " << funcName << ", in: " << file << std::endl;
		return false;
	}
	return true;
}