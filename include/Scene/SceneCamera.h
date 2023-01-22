#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

enum class Camera_Movement
{
	FORWARD		= 0,
	BACKWARD	= 1,
	LEFT		= 2,
	RIGHT		= 3
};

// Camera class used to provide view matrix and allows input to alter position
class SceneCamera
{
public:
	SceneCamera(glm::vec3 position = glm::vec3(0.0f, 2.0f, 0.0f));
    ~SceneCamera();

    void				Update();

	const glm::mat4&	GetViewMatrix() const { return m_lookAt; }
	const glm::vec3&	GetPosition() const { return m_position; }
	const glm::vec3&	GetFront() const { return m_front; }

	void				SetPosition(const glm::vec3& newPos);
    
private:

	inline void			ProcessKeyboard(Camera_Movement direction);
	inline void			ProcessMouse(float xOffset, float yOffset);

	inline void			UpdateCameraVectors();
	inline void			UpdateLookatMatrix();

    // Camera Attributes
    glm::vec3			m_position;
    glm::vec3			m_front;
    glm::vec3			m_up;
    glm::vec3			m_right;
    glm::vec3			m_worldUp;

	glm::mat4			m_lookAt;

    // Euler Angles
    float				m_yaw;
    float				m_pitch;

    // Camera Options
	float				m_movementSpeed;
	float				m_mouseSensitivity;

	bool				m_cameraMoved;
};
