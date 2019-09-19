#include "Test.h"
#include "imgui/imgui.h"

namespace test {

	TestMenu::TestMenu(Test*& p_CurrentTest)
		:m_CurrentTest(p_CurrentTest)
	{
	}
	TestMenu::~TestMenu() {}

	void TestMenu::OnImGuiRender(){
		for (auto& it : m_Tests){
			if (ImGui::Button(it.first.c_str())){
				m_CurrentTest = it.second();
			}
		}
	}

}