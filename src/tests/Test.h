#pragma once

#include <iostream>

#include <vector>
#include <functional>

#include "Observer.h"

extern const int g_WindowWidth;
extern const int g_WindowHeight;

struct GLFWwindow;
namespace test{

	class Test :public Observer {
	protected:
		Subject* m_Subject;
		GLFWwindow* m_Window;
	public:
		Test() :m_Subject(nullptr) {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void SetInputMouseInputMode() {}

		virtual void OnNotify(int event_) {}
		virtual void OnNotify(float Xevent, float Yevent, bool scroll) {}
		
		void SetWindow(GLFWwindow* window);
	};

	class TestMenu : public Test {
	private:
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
		Test*& m_CurrentTest;

	public:
		TestMenu(Test*& p_CurrentTest, Subject* const subject);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name){
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	};

}