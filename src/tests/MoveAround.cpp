#include "MoveAround.h"

#include "Renderer.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	MoveAround::MoveAround()
		:m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Model(glm::mat4(1.0f)),
		m_VertexBuffer(nullptr), m_VertexLayout(nullptr),
		m_IndexBuffer(nullptr), m_VertexArray(nullptr),
		m_Texture(nullptr), m_Shader(nullptr),
		m_Camera(nullptr)
	{
		float vertices[] = {
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, //3
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, //7

			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, //11
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, //15

			-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, //19
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f  //23
		};
		unsigned int indices[]{
			 0,  1,  2, //front
			 2,  3,  0,
			 4,  8, 11, //left
			11, 15,  4,		
			21, 12, 23, //back
			23, 22, 21,
			 9, 13, 14, //right
			14, 10,  9,
			 6,  7, 19, //top
			19, 18,  6,
			17, 16, 20, //bottom
			20,  5, 17
		};

		m_VertexArray = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 24 * 5 * sizeof(float));

		m_VertexLayout = std::make_unique<VertexBufferLayout>();
		m_VertexLayout->PushAttrib<float>(3);	//position attribute		
		m_VertexLayout->PushAttrib<float>(2);	//texture coordinates

		m_VertexArray->AddBufferLayout(*m_VertexBuffer, *m_VertexLayout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 12);

		//m_Shader = std::make_unique<Shader>("res/shaders/simple.vs", "res/shaders/simple.fs");
		m_Shader = std::make_unique<Shader>("res/shaders/3DTextureShader.shader");

		//m_Texture = std::make_unique<Texture>("res/textures/turkey-flag-icon-256.png");
		//m_Texture = std::make_unique<Texture>("res/textures/OpenGL_170px_June16.png");
		m_Texture = std::make_unique<Texture>("res/textures/mario-question-block-128.png");

		m_Texture->Bind();
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_TextureSlot", 0); // the slot id should be the same as the slot we bind our texture to

		m_Models = { 
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		m_Camera = std::make_unique<Camera>();
	}
	MoveAround::~MoveAround() {
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	void MoveAround::SetInputMouseInputMode() {
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	};

	void MoveAround::OnUpdate(float deltaTime) {}

	void MoveAround::OnRender() {

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		m_Camera->SetDeltaTime((float)glfwGetTime());

		//m_Proj = glm::ortho(0.0f, (float)g_WindowWidth, 0.0f, (float)g_WindowHeight, -1.0f, 1.0f);				//640x480 pixel window, ortho proj
		m_Proj = glm::perspective(glm::radians(45.0f), (float)g_WindowWidth / g_WindowHeight, 0.1f, 100.0f);

		Renderer renderer;
		
		//m_Model = glm::translate(glm::mat4(1.0f), m_PosA);
		float time = (float)glfwGetTime();
		for (unsigned int i = 0; i < m_Models.size(); i++) {

			m_View = m_Camera->GetViewMatrix();

			m_Model = glm::translate(glm::mat4(1.0f), m_Models[i])
					* glm::rotate(glm::mat4(1.0f), glm::radians(20.0f * i), glm::vec3(0.5f, 1.0f, 0.0f));
		
			glm::mat4 mvp = m_Proj * m_View * m_Model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
		}
	}

	void MoveAround::OnImGuiRender() {
		ImGui::Begin("Move around the scene with WASD keys.");
		ImGui::End();
	}

	void MoveAround::OnNotify(int event_) {
		m_Camera->OnNotify(event_);
	}
	void MoveAround::OnNotify(float Xevent, float Yevent, bool scroll) {
		m_Camera->OnNotify(Xevent, Yevent, scroll);
	}
	

}