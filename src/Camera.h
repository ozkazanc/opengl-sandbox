#pragma once
#include "Observer.h"

#include "glm/glm.hpp"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVITY = 0.15f;
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
	void ProcessMouseMovement(float xOffset, float yOffset);
	void ProcessMouseScroll(float yoffset);

	const float m_MovementSpeed;
	float m_Yaw;
	float m_Pitch;
	float m_MouseSensetivity;

	float m_Zoom;
public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& up);
	~Camera();
		
	void SetDeltaTime(float time);
	void OnNotify(int event_);
	void OnNotify(float Xevent, float Yevent, bool scroll);


	glm::mat4 GetViewMatrix();
	
	inline glm::vec3 GetCameraPosition() const { return m_Position; }
	inline float GetZoom() const { return m_Zoom; }
};