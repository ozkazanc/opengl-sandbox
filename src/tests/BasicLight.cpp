#include "BasicLight.h"

#include "Renderer.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	BasicLight::BasicLight()
		:m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Model(glm::mat4(1.0f)), 
		m_LightPos(glm::vec3(1.2f, 1.0f, 2.0f)),
		m_VertexBuffer(nullptr), m_VertexLayout(nullptr),
		m_IndexBuffer(nullptr), m_VertexArray(nullptr),
		m_LightingShader(nullptr),	m_Camera(nullptr),
		m_VertexBufferLightSource(nullptr), m_VertexLayoutLightSource(nullptr),
		m_IndexBufferLightSource(nullptr), m_VertexArrayLightSource(nullptr),
		m_ShaderLightSource(nullptr)
	{
		float vertices[] = {
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, //3
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0.0f, //7

			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f,  0.0f,//11
		    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f,  0.0f,//15

			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		    -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,//19
		    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f//23
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

		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 24 * 6 * sizeof(float));

		m_VertexLayout = std::make_unique<VertexBufferLayout>();
		m_VertexLayout->PushAttrib<float>(3);	//position attribute		
		m_VertexLayout->PushAttrib<float>(3);	//normal attribute		

		m_VertexArray->AddBufferLayout(*m_VertexBuffer, *m_VertexLayout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 12);

		m_LightingShader = std::make_unique<Shader>("res/shaders/BasicLighting.shader");


		m_Camera = std::make_unique<Camera>();
		GLCall(glEnable(GL_DEPTH_TEST));

		// Vertex Array for the light source
		m_VertexArrayLightSource = std::make_unique<VertexArray>();

		m_VertexBufferLightSource = std::make_unique<VertexBuffer>(vertices, 24 * 3 * sizeof(float));

		m_VertexLayoutLightSource = std::make_unique<VertexBufferLayout>();
		m_VertexLayoutLightSource->PushAttrib<float>(3);	//position attribute		

		m_VertexArrayLightSource->AddBufferLayout(*m_VertexBuffer, *m_VertexLayout);

		m_IndexBufferLightSource = std::make_unique<IndexBuffer>(indices, 3 * 12);

		m_ShaderLightSource = std::make_unique<Shader>("res/shaders/LightSource.shader");


	}
	BasicLight::~BasicLight() {
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void BasicLight::SetInputMouseInputMode() {
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	};

	void BasicLight::OnUpdate(float deltaTime) {}

	void BasicLight::OnRender() {

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float fCurrentTime = (float)glfwGetTime();
		m_Camera->SetDeltaTime(fCurrentTime);

		m_LightPos.x = sin(glm::radians(fCurrentTime * 30.0f)) * 2.0f;
		m_LightPos.z = cos(glm::radians(fCurrentTime * 30.0f)) * 1.5f;

		Renderer renderer;
		m_Proj = glm::perspective(glm::radians(m_Camera->GetZoom()), (float)g_WindowWidth / g_WindowHeight, 0.1f, 100.0f);
		m_View = m_Camera->GetViewMatrix();

		// Object
		m_Model = glm::mat4(1.0f);

		m_LightingShader->Bind();
		m_LightingShader->SetUniformMat4f("u_Projection", m_Proj);
		m_LightingShader->SetUniformMat4f("u_View", m_View);
		m_LightingShader->SetUniformMat4f("u_Model", m_Model);

		m_LightingShader->SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
		m_LightingShader->SetUniform3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
		m_LightingShader->SetUniform3f("u_LightPosition", m_LightPos.x, m_LightPos.y, m_LightPos.z);

		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_LightingShader);
	
		// Light Source
		m_Model = 
			//glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f))
			//* 
			glm::translate(glm::mat4(1.0f), m_LightPos)
			* 
			glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));


		m_ShaderLightSource->Bind();
		m_ShaderLightSource->SetUniformMat4f("u_Projection", m_Proj);
		m_ShaderLightSource->SetUniformMat4f("u_View", m_View);
		m_ShaderLightSource->SetUniformMat4f("u_Model", m_Model);


		renderer.Draw(*m_VertexArrayLightSource, *m_IndexBufferLightSource, *m_ShaderLightSource);
		
	}

	void BasicLight::OnImGuiRender() {
		ImGui::SetWindowPos(ImVec2(g_WindowWidth - 170.0f, 0.0f));
		ImGui::SetWindowSize(ImVec2(170.0f, 50.0f));
	}

	void BasicLight::OnNotify(int event_) {
		m_Camera->OnNotify(event_);
	}
	void BasicLight::OnNotify(float Xevent, float Yevent, bool scroll) {
		m_Camera->OnNotify(Xevent, Yevent, scroll);
	}


}