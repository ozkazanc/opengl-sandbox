#pragma once

#include "Test.h"

namespace test{
	
	class ColorClearTest : public Test{
	private:
		float m_ClearColor[4];

	public:
		ColorClearTest();
		~ColorClearTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};

}