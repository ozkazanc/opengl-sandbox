#include "Test.h"
#include "imgui/imgui.h"

namespace test {

	TestMenu::TestMenu(Test*& p_CurrentTest, Subject* const subject)
		:m_CurrentTest(p_CurrentTest)
	{
		m_Subject = subject;
		m_Subject->AddObserver(this);
	}
	TestMenu::~TestMenu() {}

	void TestMenu::OnImGuiRender(){
		for (auto& it : m_Tests){
			if (ImGui::Button(it.first.c_str())){
				m_CurrentTest = it.second();
				m_Subject->RemoveObserver(this);
				m_Subject->AddObserver(m_CurrentTest);
				m_CurrentTest->SetWindow(m_Window);
				m_CurrentTest->SetInputMouseInputMode();
			}
		}
	}
	void Test::SetWindow(GLFWwindow* window) {
		m_Window = window;
	}

}