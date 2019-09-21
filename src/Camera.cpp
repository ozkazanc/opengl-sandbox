#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
//#include "GLFW/glfw3.h"

#define GLFW_KEY_A                  65
#define GLFW_KEY_D                  68
#define GLFW_KEY_S                  83
#define GLFW_KEY_W                  87

Camera::Camera() 
	:m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
	 m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	 m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	 m_MovementSpeed(SPEED),
	 m_CurrentTime(0.0f), m_LastTime(0.0f), m_DeltaTime(0.0f)
{
	UpdateCameraVectors();
}
Camera::Camera(const glm::vec3& position, const glm::vec3& up)
	:m_Position(position), 
	 m_WorldUp(up),
	 m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	 m_MovementSpeed(SPEED),
	 m_CurrentTime(0.0f), m_LastTime(0.0f), m_DeltaTime(0.0f)
{
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
	glm::vec3 front;

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

}

Camera::~Camera() {}

void Camera::OnNotify(int event_) {
	//SetDeltaTime();
	if (event_ == GLFW_KEY_A) {
		m_Position -= m_Right * m_MovementSpeed * m_DeltaTime;
	}
	if (event_ == GLFW_KEY_D) {
		m_Position += m_Right * m_MovementSpeed * m_DeltaTime;
	}
	if (event_ == GLFW_KEY_S) {
		m_Position -= m_Front * m_MovementSpeed * m_DeltaTime;
	}
	if (event_ == GLFW_KEY_W) {
		m_Position += m_Front * m_MovementSpeed * m_DeltaTime;
	}
}

void Camera::SetDeltaTime(float time) {
	m_CurrentTime = time;
	m_DeltaTime = m_CurrentTime - m_LastTime;
	m_LastTime = m_CurrentTime;
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}



