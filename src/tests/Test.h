#pragma once

#include <iostream>

#include <vector>
#include <functional>

#include "Observer.h"

namespace test{

	class Test :public Observer {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void OnNotify(int event_) {}
	};

	class TestMenu : public Test {
	private:
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
		Test*& m_CurrentTest;

	public:
		TestMenu(Test*& p_CurrentTest);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name){
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	};

}