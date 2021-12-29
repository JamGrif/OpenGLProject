#pragma once



#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	e_FORWARD,
	e_BACKWARD,
	e_LEFT,
	e_RIGHT
};

//Default camera values
constexpr GLfloat YAW = -90.0f;
constexpr GLfloat PITCH = 0.0f;
constexpr GLfloat SPEED = 14.0f;
constexpr GLfloat SENSITIVTY = 0.25f;

//Camera class used to provide view matrix and allows input to alter position
class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
    ~Camera();

    void            Update(double deltaTime);

    glm::mat4       getViewMatrix() const;
    glm::vec3       getPosition() const;
    glm::vec3       getFront() const;
    
    void            processKeyboard(Camera_Movement direction, double deltaTime);
    void            processMouse(float xOffset, float yOffset, bool constrainPitch = true);

private:

    void            updateCameraVectors();

    //Camera Attributes
    glm::vec3       m_position;
    glm::vec3       m_front;
    glm::vec3       m_up;
    glm::vec3       m_right;
    glm::vec3       m_worldUp;

    //Euler Angles
    float         m_yaw;
    float         m_pitch;

    //Camera Options
	float         m_movementSpeed;
	float         m_mouseSensitivity;
	float         m_zoom;
};
