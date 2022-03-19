#include "Camera.h"

#include <iostream>

#include "Input.h"
#include "EngineStatics.h"
#include "GameTimer.h"

Camera::Camera(glm::vec3 position)
    : m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_up(glm::vec3(0.0f,1.0f,0.0f)), m_right(glm::vec3(0.0f,0.0f,0.0f)), m_worldUp(m_up), m_lookAt(1.0f),
	m_yaw(DEFAULT_YAW), m_pitch(DEFAULT_PITCH), m_movementSpeed(DEFAULT_SPEED), m_mouseSensitivity(DEFAULT_SENSITIVTY), m_cameraMoved(false)
{
    std::cout << "Scene Camera Initialized" << std::endl;

	// Initialize default values
    updateCameraVectors();
	m_lookAt = glm::lookAt(m_position, m_position + m_front, m_up); 

	m_localGameTimer = EngineStatics::getGameTimer();
}

Camera::~Camera()
{
    std::cout << "Scene Camera Destroyed" << std::endl;

	m_localGameTimer = nullptr;
}

/// <summary>
/// Updates the camera depending on input from the keyboard or mouse
/// </summary>
/// <param name="deltaTime"></param>
void Camera::Update()
{
	m_cameraMoved = false;

	if (Input::getKeyPressed(GLFW_KEY_W)) { processKeyboard(e_FORWARD); }
	if (Input::getKeyPressed(GLFW_KEY_S)) { processKeyboard(e_BACKWARD); }
	if (Input::getKeyPressed(GLFW_KEY_A)) { processKeyboard(e_LEFT); }
	if (Input::getKeyPressed(GLFW_KEY_D)) { processKeyboard(e_RIGHT); }

    if (Input::getKeyPressed(GLFW_KEY_R))
    {
        std::cout << "Position x - " << m_position.x << " Position y - " << m_position.y << " Position z - " << m_position.z << std::endl;
    }

	// Only check for mouse movement if cursor is disabled
	if (!Input::getMouseEnabled())
	{
		// Get mouse movement
		double x, y;
		Input::getMouseMoved(x, y);

		// Only process mouse movement if mouse has been moved
		if (x != 0 || y != 0)
		{
			processMouse(static_cast<float>(x), static_cast<float>(y));
		}
		
	}
    
	// Calculate lookAt matrix
	if (m_cameraMoved)
	{
		m_lookAt = glm::lookAt(m_position, m_position + m_front, m_up);
	}
}

const glm::mat4& Camera::getViewMatrix() const
{
	return m_lookAt;
}

const glm::vec3& Camera::getPosition() const
{
    return m_position;
}

const glm::vec3& Camera::getFront() const
{
    return m_front;
}

/// <summary>
/// Moves the camera depending on user keyboard input
/// </summary>
/// <param name="direction">Direction the camera is moving</param>
void Camera::processKeyboard(Camera_Movement direction)
{
	m_cameraMoved = true;
	float velocity = m_movementSpeed * static_cast<float>(m_localGameTimer->getDeltaTime());

    //Multiple if statements to allow multiple keys pressed down
    if (direction == e_FORWARD)
    {
        m_position += m_front * velocity;
    }

    if (direction == e_BACKWARD)
    {
        m_position -= m_front * velocity;
    }

    if (direction == e_LEFT)
    {
        m_position -= m_right * velocity;
    }

    if (direction == e_RIGHT)
    {
        m_position += m_right * velocity;
    }
}

/// <summary>
/// Moves the camera depending on user mouse input
/// </summary>
/// <param name="xOffset"></param>
/// <param name="yOffset"></param>
void Camera::processMouse(float xOffset, float yOffset)
{
	m_cameraMoved = true;
    
    if (xOffset > 100 && xOffset > 0)
        xOffset = 100;
    if (yOffset > 100 && yOffset > 0)
        yOffset = 100;
    
    if (xOffset < -100 && xOffset < 0)
        xOffset = -100;
    if (yOffset < -100 && yOffset < 0)
        yOffset = -100;
    
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;
    
    m_yaw += xOffset;
    m_pitch += yOffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0;
    }
    if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }
    
    // Update Front, Right and Up vectors using the updated Eular angles
    updateCameraVectors();
}

/// <summary>
/// Updates the vectors of the camera after camera moves from input
/// </summary>
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    //glm::vec3 front = { 0.0f,0.0f,0.0f };
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);
    
    // Also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
