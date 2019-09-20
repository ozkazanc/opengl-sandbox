#pragma once

#include "Test.h"

#include <memory>
namespace test{
	
	class Texture2D : public Test{
	private:
		//va,vb,ib,shader,texture,renderer
	public:
		Texture2D();
		~Texture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};

}