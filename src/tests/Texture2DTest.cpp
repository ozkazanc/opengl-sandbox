#include "Texture2DTest.h"
#include "Renderer.h"
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test{

	Texture2DTest::Texture2DTest() 
		:m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Model(glm::mat4(1.0f)),
		m_ColorA(1.0f), m_ColorB(1.0f), m_PosA(1.0f), m_PosB(1.0f),
		m_VertexBuffer(nullptr), m_VertexLayout(nullptr),
		m_IndexBuffer(nullptr), m_VertexArray(nullptr),
		m_Texture(nullptr), m_Shader(nullptr)
	{
		float vertices[] = {
			-40.0f, -40.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			40.0f, -40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			40.0f, 40.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		unsigned int indices[]{
			0, 1, 2,
			2, 3, 0,
		};

		m_VertexArray = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 7 * sizeof(float));

		m_VertexLayout = std::make_unique<VertexBufferLayout>();
		m_VertexLayout->PushAttrib<float>(2);	//position attribute		
		m_VertexLayout->PushAttrib<float>(3);	//color attribute
		m_VertexLayout->PushAttrib<float>(2);	//texture coordinates

		m_VertexArray->AddBufferLayout(*m_VertexBuffer, *m_VertexLayout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		//m_Shader = std::make_unique<Shader>("res/shaders/simple.vs", "res/shaders/simple.fs");
		m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");

		m_Texture = std::make_unique<Texture>("res/textures/turkey-flag-icon-256.png");
		//m_Texture = std::make_unique<Texture>("res/textures/OpenGL_170px_June16.png");
		//m_Texture = std::make_unique<Texture>("res/textures/mario-question-block-128.png");
		
		m_Texture->Bind();
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_TextureSlot", 0); // the slot id should be the same as the slot we bind our texture to

		m_PosA = glm::vec3(80.0f, 100.0f, 0.0f);
		m_PosB = glm::vec3(500.0f, 100.0f, 0.0f);
		m_ColorA = glm::vec3(0.0f);
		m_ColorB = glm::vec3(0.0f);
	}
	Texture2DTest::~Texture2DTest() {}

	void Texture2DTest::OnUpdate(float deltaTime) {}

	void Texture2DTest::OnRender(){
		m_Proj = glm::ortho(0.0f, (float)g_WindowWidth, 0.0f, (float)g_WindowHeight, -1.0f, 1.0f);				//640x480 pixel window, ortho proj
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));									//view(camera) matrix
		
		Renderer renderer;
		{
			//Render the first block
			m_Model = glm::translate(glm::mat4(1.0f), m_PosA);
			glm::mat4 mvp = m_Proj * m_View * m_Model;
			
			m_Shader->Bind();
			m_Shader->SetUniform3f("u_Color", m_ColorA.x, m_ColorA.y, m_ColorA.z);
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
		{
			//Render the first block
			m_Model = glm::translate(glm::mat4(1.0f), m_PosB);
			glm::mat4 mvp = m_Proj * m_View * m_Model;

			m_Shader->Bind();
			m_Shader->SetUniform3f("u_Color", m_ColorB.x, m_ColorB.y, m_ColorB.z);
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
	}
	
	void Texture2DTest::OnImGuiRender(){
		ImGui::SliderFloat("XPositionA", &m_PosA.x, 0.0f, 640.0f);
		ImGui::SliderFloat("YPositionA", &m_PosA.y, 0.0f, 480.0f);
		ImGui::SliderFloat("XPositionB", &m_PosB.x, 0.0f, 640.0f);
		ImGui::SliderFloat("YPositionB", &m_PosB.y, 0.0f, 480.0f);

		ImGui::SliderFloat3("ColorA", glm::value_ptr(m_ColorA), -1.0f, 1.0f);
		ImGui::SliderFloat3("ColorB", glm::value_ptr(m_ColorB), -1.0f, 1.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}