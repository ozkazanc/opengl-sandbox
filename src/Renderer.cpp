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

void Renderer::Clear() const {
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f)); // hard coded buffer color for now
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
	va.Bind();
	ib.Bind();
	shader.Bind();
	
	//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}