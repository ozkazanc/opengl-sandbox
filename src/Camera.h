#pragma once
#include "Observer.h"

#include "glm/glm.hpp"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera : public Observer{
private:
	glm::vec3 m_Position;
	glm::vec3 m_WorldUp;
	
	glm::vec3 m_Front;
	glm::vec3 m_Right;
	glm::vec3 m_Up;
	
	float m_CurrentTime;
	float m_LastTime;
	float m_DeltaTime;

	void UpdateCameraVectors();

	const float m_MovementSpeed;
	float m_Yaw;
	float m_Pitch;
	float m_MouseSensetivity;

public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& up);
	~Camera();
		
	void SetDeltaTime(float time);
	void OnNotify(int event_);
	void OnNotify(float Xevent, float Yevent, bool scroll);

	void MouseMovement(float xOffset, float yOffset);
	glm::mat4 GetViewMatrix();
};