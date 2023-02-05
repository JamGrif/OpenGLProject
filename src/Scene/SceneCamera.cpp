#include "pch.h"
#include "Scene/SceneCamera.h"

#include "Core/InputHandler.h"

#include <glm/gtc/matrix_transform.hpp>

// Default camera values
constexpr float Default_YAW = -90.0f;
constexpr float Default_PITCH = 0.0f;
constexpr float Default_SPEED = 14.0f;
constexpr float Default_SENSITIVTY = 0.25f;

SceneCamera::SceneCamera(Vector3D position)
	: m_position(glm::vec3(position.GetX(), position.GetY(), position.GetZ())),
	m_front(0.0f, 0.0f, -1.0f), m_up(0.0f, 1.0f, 0.0f), m_right(0.0f, 0.0f, 0.0f), m_worldUp(m_up), m_lookAt(1.0f),
	m_yaw(Default_YAW), m_pitch(Default_PITCH), m_movementSpeed(Default_SPEED), m_mouseSensitivity(Default_SENSITIVTY), m_bCameraMoved(false)
{
	// Initialize default values
	UpdateCameraVectors();
	UpdateLookatMatrix();
}

SceneCamera::~SceneCamera()
{
}

/// <summary>
/// Updates the camera depending on input from the keyboard or mouse
/// </summary>
void SceneCamera::Update()
{
	m_bCameraMoved = false;

	if (InputHandler::Get()->GetKeyPressed(Keyboard::W))
		ProcessKeyboard(CameraMovement::FORWARD);
	if (InputHandler::Get()->GetKeyPressed(Keyboard::S))
		ProcessKeyboard(CameraMovement::BACKWARD);
	if (InputHandler::Get()->GetKeyPressed(Keyboard::A))
		ProcessKeyboard(CameraMovement::LEFT);
	if (InputHandler::Get()->GetKeyPressed(Keyboard::D))
		ProcessKeyboard(CameraMovement::RIGHT);

    if (InputHandler::Get()->GetKeyPressed(Keyboard::R))
		PRINT_TRACE("Position x - {0}, Position y - {1}, Position z - {2}", m_position.x, m_position.y, m_position.z);
    
	// Only check for mouse movement if cursor is disabled
	if (!InputHandler::Get()->GetMouseEnabled())
	{
		// Get mouse movement
		double x, y;
		InputHandler::Get()->GetMouseMoved(x, y);

		// Only process mouse movement if mouse has been moved
		if (x != 0 || y != 0)
			ProcessMouse(static_cast<float>(x), static_cast<float>(y));
	}
    
	// Calculate lookAt matrix if camera has moved this frame
	if (m_bCameraMoved)
		UpdateLookatMatrix();
}

/// <summary>
/// Updates the cameras positions
/// </summary>
void SceneCamera::SetPosition(const Vector3D& newPos)
{
	m_position.x = newPos.GetX();
	m_position.y = newPos.GetY();
	m_position.z = newPos.GetZ();

	UpdateCameraVectors();
	UpdateLookatMatrix();
}

/// <summary>
/// Moves the camera depending on user keyboard input
/// </summary>
void SceneCamera::ProcessKeyboard(CameraMovement direction)
{
	m_bCameraMoved = true;
	float velocity = m_movementSpeed * static_cast<float>(ApplicationClock::Get()->GetDeltaTime());

    // Multiple if statements to allow multiple keys pressed down
    if (direction == CameraMovement::FORWARD)
		m_position += m_front * velocity;
    
    if (direction == CameraMovement::BACKWARD)
		m_position -= m_front * velocity;
    
    if (direction == CameraMovement::LEFT)
		m_position -= m_right * velocity;
    
    if (direction == CameraMovement::RIGHT)
		m_position += m_right * velocity;
}

/// <summary>
/// Moves the camera depending on user mouse input
/// </summary>
void SceneCamera::ProcessMouse(float xOffset, float yOffset)
{
	m_bCameraMoved = true;

	// Clamp speed
	if (xOffset > 100)
		xOffset = 100;
	else if (xOffset < -100)
		xOffset = -100;

	if (yOffset > 100)
		yOffset = 100;
	else if (yOffset < -100)
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
    UpdateCameraVectors();
}

/// <summary>
/// Updates the vectors of the camera after camera moves from input
/// </summary>
void SceneCamera::UpdateCameraVectors()
{
    // Calculate the new Front vector
    m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front.y = sin(glm::radians(m_pitch));
    m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);
    
    // Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void SceneCamera::UpdateLookatMatrix()
{
	m_lookAt = glm::lookAt(m_position, m_position + m_front, m_up);
}
