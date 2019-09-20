#include "ControlColorWithKeyboardTest.h"
#include "Renderer.h"
#include "GL/glew.h"
#include "imgui/imgui.h"

#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
namespace test{

	ControlColorWithKeyboardTest::ControlColorWithKeyboardTest()
		//:m_Color{ 0.0f, 0.0f, 0.0f, 1.0f }
	{
		m_Color[0] = 0.0f, m_Color[1] = 0.0f, m_Color[2] = 0.0f, m_Color[3] = 1.0f;
	}
	ControlColorWithKeyboardTest::~ControlColorWithKeyboardTest() {}

	void ControlColorWithKeyboardTest::OnUpdate(float deltaTime) {}
	
	void ControlColorWithKeyboardTest::OnRender() {
		GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	
	void ControlColorWithKeyboardTest::OnImGuiRender() {
		ImGui::Begin("Select channel and use up/down arrow keys to adjust color");
		
		ImGui::Checkbox("R", &m_Channels[0]);
		ImGui::Checkbox("G", &m_Channels[1]);
		ImGui::Checkbox("B", &m_Channels[2]);
		
		ImGui::Text("R: %d, G: %d, B: %d", int(255 * m_Color[0]), int(255 * m_Color[1]), int(255* m_Color[2]));

		ImGui::End();
	}

	void ControlColorWithKeyboardTest::OnNotify(int event_) {
		const float increment = 0.05f;
		for (int i = 0; i < 3; i++){

			if (m_Channels[i] && event_ == GLFW_KEY_DOWN){
				m_Color[i] -= increment;
				if (m_Color[i] < 0.0f)
					m_Color[i] = 0.0f;
			}
			
			else if (m_Channels[i] && event_ == GLFW_KEY_UP){
				m_Color[i] += increment;
				if (m_Color[i] > 1.0f)
					m_Color[i] = 1.0f;

			}	
		}
	}

}