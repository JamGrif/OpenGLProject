#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

enum Camera_Movement
{
	e_FORWARD,
	e_BACKWARD,
	e_LEFT,
	e_RIGHT
};


// Forward Declarations
class GameTimer;

// Default camera values
constexpr GLfloat DEFAULT_YAW = -90.0f;
constexpr GLfloat DEFAULT_PITCH = 0.0f;
constexpr GLfloat DEFAULT_SPEED = 14.0f;
constexpr GLfloat DEFAULT_SENSITIVTY = 0.25f;

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

	// Cached other classes
	std::shared_ptr<GameTimer>	m_localGameTimer;
};
