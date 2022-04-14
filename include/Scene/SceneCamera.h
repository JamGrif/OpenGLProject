#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

enum class Camera_Movement
{
	e_FORWARD	= 0,
	e_BACKWARD	= 1,
	e_LEFT		= 2,
	e_RIGHT		= 3
};

// Camera class used to provide view matrix and allows input to alter position
class SceneCamera
{
public:
	SceneCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
    ~SceneCamera();

    void				Update();

	const glm::mat4&	getViewMatrix() const;
	const glm::vec3&	getPosition() const;
	const glm::vec3&	getFront() const;

	void				setPosition(const glm::vec3& newPos);
    
private:

	inline void			processKeyboard(Camera_Movement direction);
	inline void			processMouse(float xOffset, float yOffset);

    inline void			updateCameraVectors();
	inline void			updateLookatMatrix();

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
