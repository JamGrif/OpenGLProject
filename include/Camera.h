#pragma once

#include <glm/glm.hpp>

enum Camera_Movement
{
	e_FORWARD,
	e_BACKWARD,
	e_LEFT,
	e_RIGHT
};

// Camera class used to provide view matrix and allows input to alter position
class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
    ~Camera();

    void				Update();

	const glm::mat4&	getViewMatrix() const;
	const glm::vec3&	getPosition() const;
	const glm::vec3&	getFront() const;
    
    void				processKeyboard(Camera_Movement direction);
    void				processMouse(float xOffset, float yOffset);

private:

    inline void			updateCameraVectors();

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
