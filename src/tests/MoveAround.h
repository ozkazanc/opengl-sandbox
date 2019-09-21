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

extern const int g_WindowWidth;
extern const int g_WindowHeight;
namespace test {

	class MoveAround : public Test
	{
	private:
		glm::mat4 m_Proj, m_View, m_Model;
		std::vector<glm::vec3> m_Models;

		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexBufferLayout> m_VertexLayout;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		std::unique_ptr<Camera> m_Camera;

	public:
		MoveAround();
		~MoveAround();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void OnNotify(int event_) override;

	};

}
