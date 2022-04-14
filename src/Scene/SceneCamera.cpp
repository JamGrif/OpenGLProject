#include "pch.h"
#include "Scene/SceneCamera.h"

#include "Core/EngineStatics.h"

#include <glm/gtc/matrix_transform.hpp>

// Default camera values
constexpr float Default_YAW = -90.0f;
constexpr float Default_PITCH = 0.0f;
constexpr float Default_SPEED = 14.0f;
constexpr float Default_SENSITIVTY = 0.25f;

SceneCamera::SceneCamera(glm::vec3 position)
    : m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_up(glm::vec3(0.0f,1.0f,0.0f)), m_right(glm::vec3(0.0f,0.0f,0.0f)), m_worldUp(m_up), m_lookAt(1.0f),
	m_yaw(Default_YAW), m_pitch(Default_PITCH), m_movementSpeed(Default_SPEED), m_mouseSensitivity(Default_SENSITIVTY), m_cameraMoved(false)
{
	//PRINT_INFO("Scene Camera Initialized");

	// Initialize default values
    updateCameraVectors();
	updateLookatMatrix();
	//m_lookAt = glm::lookAt(m_position, m_position + m_front, m_up); 
}

SceneCamera::~SceneCamera()
{
	//PRINT_INFO("Scene Camera Destroyed");
}

/// <summary>
/// Updates the camera depending on input from the keyboard or mouse
/// </summary>
void SceneCamera::Update()
{
	m_cameraMoved = false;

	if (Input::getKeyPressed(Keyboard::W)) { processKeyboard(Camera_Movement::e_FORWARD); }
	if (Input::getKeyPressed(Keyboard::S)) { processKeyboard(Camera_Movement::e_BACKWARD); }
	if (Input::getKeyPressed(Keyboard::A)) { processKeyboard(Camera_Movement::e_LEFT); }
	if (Input::getKeyPressed(Keyboard::D)) { processKeyboard(Camera_Movement::e_RIGHT); }

    if (Input::getKeyPressed(Keyboard::R))
		PRINT_TRACE("Position x - {0}, Position y - {1}, Position z - {2}", m_position.x, m_position.y, m_position.z);
    

	// Only check for mouse movement if cursor is disabled
	if (!Input::getMouseEnabled())
	{
		// Get mouse movement
		double x, y;
		Input::getMouseMoved(x, y);

		// Only process mouse movement if mouse has been moved
		if (x != 0 || y != 0)
			processMouse(static_cast<float>(x), static_cast<float>(y));
	}
    
	// Calculate lookAt matrix
	if (m_cameraMoved)
		updateLookatMatrix();
		//m_lookAt = glm::lookAt(m_position, m_position + m_front, m_up);
}

const glm::mat4& SceneCamera::getViewMatrix() const
{
	return m_lookAt;
}

const glm::vec3& SceneCamera::getPosition() const
{
    return m_position;
}

const glm::vec3& SceneCamera::getFront() const
{
    return m_front;
}

/// <summary>
/// Updates the cameras positions and refreshes the cameras vectors as well
/// </summary>
void SceneCamera::setPosition(const glm::vec3& newPos)
{
	m_position = newPos;

	updateCameraVectors();
	updateLookatMatrix();
}

/// <summary>
/// Moves the camera depending on user keyboard input
/// </summary>
/// <param name="direction">Direction the camera is moving</param>
void SceneCamera::processKeyboard(Camera_Movement direction)
{
	m_cameraMoved = true;
	float velocity = m_movementSpeed * static_cast<float>(ApplicationClock::getDeltaTime());

    // Multiple if statements to allow multiple keys pressed down
    if (direction == Camera_Movement::e_FORWARD)
		m_position += m_front * velocity;
    
    if (direction == Camera_Movement::e_BACKWARD)
		m_position -= m_front * velocity;
    
    if (direction == Camera_Movement::e_LEFT)
		m_position -= m_right * velocity;
    
    if (direction == Camera_Movement::e_RIGHT)
		m_position += m_right * velocity;
}

/// <summary>
/// Moves the camera depending on user mouse input
/// </summary>
/// <param name="xOffset">X mouse movement</param>
/// <param name="yOffset">Y mouse movement</param>
void SceneCamera::processMouse(float xOffset, float yOffset)
{
	m_cameraMoved = true;

	// Stop mouse from moving too fast
	if (xOffset > 100)
		xOffset = 100;
	
	if (yOffset > 100)
		yOffset = 100;
	
	if (xOffset < -100)
		xOffset = -100;

	if (yOffset < -100)
		yOffset = -100;
	
	// Ensure mouse only moves by the sensitivity
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;
    
    m_yaw += xOffset;
    m_pitch += yOffset;
    
	// Constrain pitch from flipping the screen
    if (m_pitch > 89.0f)
		m_pitch = 89.0;
    
    if (m_pitch < -89.0f)
		m_pitch = -89.0f;
    
    // Update Front, Right and Up vectors using the updated Eular angles
    updateCameraVectors();
}

/// <summary>
/// Updates the vectors of the camera after camera moves from input
/// </summary>
void SceneCamera::updateCameraVectors()
{
    // Calculate the new Front vector
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);
    
    // Also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void SceneCamera::updateLookatMatrix()
{
	m_lookAt = glm::lookAt(m_position, m_position + m_front, m_up);
}
