#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

enum class CameraMovement
{
	FORWARD		= 0,
	BACKWARD	= 1,
	LEFT		= 2,
	RIGHT		= 3
};

/// <summary>
/// Provides a view matrix for rendering that can be manipulated on user input
/// </summary>
class SceneCamera
{
public:
	SceneCamera(Vector3D position = Vector3D(0.0f, 2.0f, 0.0f));
	~SceneCamera();

    void				Update();

	const glm::mat4&	GetViewMatrix() const { return m_lookAt; }
	const glm::vec3&	GetPosition() const { return m_position; }
	const glm::vec3&	GetFront() const { return m_front; }

	void				SetPosition(const Vector3D& newPos);
    
private:

	inline void			ProcessKeyboard(CameraMovement direction);
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

	bool				m_bCameraMoved;
};
