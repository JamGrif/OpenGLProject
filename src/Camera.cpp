#include "Camera.h"

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY),
      m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch)
{
    std::cout << "Scene Camera Initialized" << std::endl;

    this->updateCameraVectors();

    //EngineStatics::Setcamera(this);
}

Camera::~Camera()
{
    std::cout << "Scene Camera Destroyed" << std::endl;

    //EngineStatics::Setcamera(nullptr);
}

/// <summary>
/// Updates the camera depending on input from the keyboard or mouse
/// </summary>
/// <param name="deltaTime"></param>
void Camera::Update(GLfloat deltaTime)
{
    if (Input::getKeyPressed(GLFW_KEY_W)) { processKeyboard(FORWARD, deltaTime); }
    if (Input::getKeyPressed(GLFW_KEY_S)) { processKeyboard(BACKWARD, deltaTime); }
    if (Input::getKeyPressed(GLFW_KEY_A)) { processKeyboard(LEFT, deltaTime); }
    if (Input::getKeyPressed(GLFW_KEY_D)) { processKeyboard(RIGHT, deltaTime); }

    if (Input::getKeyPressed(GLFW_KEY_R))
    {
        std::cout << "Position x - " << m_position.x << " Position y - " << m_position.y << " Position z - " << m_position.z << std::endl;
    }

    //Get mouse movement
    double x, y;
    Input::getMouseMoved(x,y);
    processMouse(x, y);
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::getPosition() const
{
    return m_position;
}

glm::vec3 Camera::getFront() const
{
    return m_front;
}

/// <summary>
/// Moves the camera depending on user keyboard input
/// </summary>
/// <param name="direction">Direction the camera is moving</param>
/// <param name="deltaTime"></param>
void Camera::processKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    //std::cout << "process keyboard" << std::endl;
	GLfloat velocity = m_movementSpeed * deltaTime;


    //Multiple if statements to allow multiple keys pressed down
    if (direction == FORWARD)
    {
        m_position += m_front * velocity;
    }

    if (direction == BACKWARD)
    {
        m_position -= m_front * velocity;
    }

    if (direction == LEFT)
    {
        m_position -= m_right * velocity;
    }

    if (direction == RIGHT)
    {
        m_position += m_right * velocity;
    }
}

/// <summary>
/// Moves the camera depending on user mouse input
/// </summary>
/// <param name="xOffset"></param>
/// <param name="yOffset"></param>
/// <param name="constrainPitch">Decides if the camera should loop around when reaching maximum pitch</param>
void Camera::processMouse(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
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
    
    //Make sure that when pitch is out of bounds, screen doesnt get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
        {
            m_pitch = 89.0;
        }
        if (m_pitch < -89.0f)
        {
            m_pitch = -89.0f;
        }
    }
    
    //Update Front, Right and Up vectors using the updated eular angles
    this->updateCameraVectors();

}

/// <summary>
/// Updates the vectors of the camera after camera moves from input
/// </summary>
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front = { 0.0f,0.0f,0.0f };
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    
    // Also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
