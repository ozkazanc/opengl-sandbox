#pragma once

#include "Test.h"

namespace test{

	class ControlColorWithKeyboardTest : public Test {
	private:
		float m_Color[4];
		bool m_Channels[3];

		float m_CurrentTime;
		float m_LastTime;
		float m_DeltaTime;
		
		void SetDeltaTime();
	public:
		ControlColorWithKeyboardTest();
		~ControlColorWithKeyboardTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void OnNotify(int event_) override;
	};
}