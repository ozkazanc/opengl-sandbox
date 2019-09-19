#include "ClearColorTest.h"
#include "Renderer.h"
#include "GL/glew.h"
#include "imgui/imgui.h"

namespace test{

	ColorClearTest::ColorClearTest()
		//:m_ClearColor {0.6f, 0.6f, 0.3f, 1.0f}
	{
		m_ClearColor[0] = 0.6f, m_ClearColor[1] = 0.6f, m_ClearColor[2] = 0.3f, m_ClearColor[3] = 1.0f;
	}
	ColorClearTest::~ColorClearTest() {}

	void ColorClearTest::OnUpdate(float deltaTime) {}

	void ColorClearTest::OnRender(){
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	
	void ColorClearTest::OnImGuiRender(){
		ImGui::Begin("Background Color");
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
		ImGui::End();
	}

}