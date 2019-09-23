#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include <memory>
#include <vector>

struct GLFWwindow;

extern const int g_WindowWidth;
extern const int g_WindowHeight;
namespace test {

	class BasicLight : public Test
	{
	private:
		glm::mat4 m_Proj, m_View, m_Model;
		glm::vec3 m_LightPos;

		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexBufferLayout> m_VertexLayout;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<Shader> m_LightingShader;


		std::unique_ptr<VertexBuffer> m_VertexBufferLightSource;
		std::unique_ptr<VertexBufferLayout> m_VertexLayoutLightSource;
		std::unique_ptr<IndexBuffer> m_IndexBufferLightSource;
		std::unique_ptr<VertexArray> m_VertexArrayLightSource;
		std::unique_ptr<Shader> m_ShaderLightSource;

		std::unique_ptr<Camera> m_Camera;

	public:
		BasicLight();
		~BasicLight();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void SetInputMouseInputMode() override;

		void OnNotify(int event_) override;
		void OnNotify(float Xevent, float Yevent, bool scroll) override;

	};

}
