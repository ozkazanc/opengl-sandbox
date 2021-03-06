#include "LightingMap.h"

#include "Renderer.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	LightingMap::LightingMap()
		:m_Proj(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Model(glm::mat4(1.0f)),
		m_LightPos(glm::vec3(1.2f, 1.0f, 2.0f)),
		m_VertexBuffer(nullptr), m_VertexLayout(nullptr),
		m_IndexBuffer(nullptr), m_VertexArray(nullptr),
		m_LightingShader(nullptr), m_Camera(nullptr),
		m_VertexBufferLightSource(nullptr), m_VertexLayoutLightSource(nullptr),
		m_IndexBufferLightSource(nullptr), m_VertexArrayLightSource(nullptr),
		m_ShaderLightSource(nullptr), 
		m_TextureDiffuseMap(nullptr), m_TextureSpecularMap(nullptr)
	{
		float vertices[] = {
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,	//3
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,	//7

			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,	//11
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,	//15

			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,	//19
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f	//23
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

		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 24 * 8 * sizeof(float));

		m_VertexLayout = std::make_unique<VertexBufferLayout>();
		m_VertexLayout->PushAttrib<float>(3);	//position attribute		
		m_VertexLayout->PushAttrib<float>(3);	//normal attribute		
		m_VertexLayout->PushAttrib<float>(2);	//texture coordinates		

		m_VertexArray->AddBufferLayout(*m_VertexBuffer, *m_VertexLayout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 12);

		m_LightingShader = std::make_unique<Shader>("res/shaders/LightingMapShader.shader");

		m_LightingShader->Bind();
		m_TextureDiffuseMap = std::make_unique<Texture>("res/textures/wooden_box_diffuse_map.png");
		m_TextureDiffuseMap->Bind(0);
		m_LightingShader->SetUniform1i("u_Material.diffuseMap", 0);

		m_LightingShader->Bind();
		m_TextureSpecularMap = std::make_unique<Texture>("res/textures/wooden_box_specular_map.png");
		m_TextureSpecularMap->Bind(1);
		m_LightingShader->SetUniform1i("u_Material.specularMap", 1);


		m_Camera = std::make_unique<Camera>();
		GLCall(glEnable(GL_DEPTH_TEST));

		// Vertex Array for the light source
		m_VertexArrayLightSource = std::make_unique<VertexArray>();

		m_VertexBufferLightSource = std::make_unique<VertexBuffer>(vertices, 24 * 8 * sizeof(float));

		m_VertexLayoutLightSource = std::make_unique<VertexBufferLayout>();
		m_VertexLayoutLightSource->PushAttrib<float>(3);	//position attribute		
		m_VertexLayoutLightSource->PushAttrib<float>(3);
		m_VertexLayoutLightSource->PushAttrib<float>(2);

		m_VertexArrayLightSource->AddBufferLayout(*m_VertexBuffer, *m_VertexLayout);

		m_IndexBufferLightSource = std::make_unique<IndexBuffer>(indices, 3 * 12);

		m_ShaderLightSource = std::make_unique<Shader>("res/shaders/LightSource.shader");

	}
	LightingMap::~LightingMap() {
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void LightingMap::SetInputMouseInputMode() {
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	};

	void LightingMap::OnUpdate(float deltaTime) {}

	void LightingMap::OnRender() {

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float fCurrentTime = (float)glfwGetTime();
		m_Camera->SetDeltaTime(fCurrentTime);

		// Rotate the light source
		m_LightPos.x = sin(glm::radians(fCurrentTime * 30.0f)) * 2.0f;
		m_LightPos.z = cos(glm::radians(fCurrentTime * 30.0f)) * 1.5f;

		Renderer renderer;
		m_Proj = glm::perspective(glm::radians(m_Camera->GetZoom()), (float)g_WindowWidth / g_WindowHeight, 0.1f, 100.0f);
		m_View = m_Camera->GetViewMatrix();

		// Object
		m_Model = glm::mat4(1.0f);

		m_LightingShader->Bind();
		
		m_TextureDiffuseMap->Bind(0);
		m_LightingShader->SetUniform1i("u_Material.diffuseMap", 0);
		m_TextureSpecularMap->Bind(1);
		m_LightingShader->SetUniform1i("u_Material.specularMap", 1);

		m_LightingShader->SetUniform1f("u_Material.shininess", 64.0f);
		
		m_LightingShader->SetUniformMat4f("u_Projection", m_Proj);
		m_LightingShader->SetUniformMat4f("u_View", m_View);
		m_LightingShader->SetUniformMat4f("u_Model", m_Model);

		m_LightingShader->SetUniform3f("u_Light.specular", 1.0f, 1.0f, 1.0f);
		m_LightingShader->SetUniform3f("u_Light.ambient", 0.2f, 0.2f, 0.2f);
		m_LightingShader->SetUniform3f("u_Light.diffuse", 0.5f, 0.5f, 0.5f);
		m_LightingShader->SetUniform3f("u_Light.position", m_LightPos.x, m_LightPos.y, m_LightPos.z);

		glm::vec3 cameraPosition = m_Camera->GetCameraPosition();
		m_LightingShader->SetUniform3f("u_ViewPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);


		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_LightingShader);

		// Light Source
		m_Model =
			glm::translate(glm::mat4(1.0f), m_LightPos)
			*
			glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));


		m_ShaderLightSource->Bind();
		m_ShaderLightSource->SetUniformMat4f("u_Projection", m_Proj);
		m_ShaderLightSource->SetUniformMat4f("u_View", m_View);
		m_ShaderLightSource->SetUniformMat4f("u_Model", m_Model);


		renderer.Draw(*m_VertexArrayLightSource, *m_IndexBufferLightSource, *m_ShaderLightSource);
	}

	void LightingMap::OnImGuiRender() {
		ImGui::SetWindowPos(ImVec2(g_WindowWidth - 170.0f, 0.0f));
		ImGui::SetWindowSize(ImVec2(170.0f, 60.0f));
	}

	void LightingMap::OnNotify(int event_) {
		m_Camera->OnNotify(event_);
	}
	void LightingMap::OnNotify(float Xevent, float Yevent, bool scroll) {
		m_Camera->OnNotify(Xevent, Yevent, scroll);
	}
}