#pragma once

#include "Input.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 14.0f;
const GLfloat SENSITIVTY = 0.25f;

//Camera class used to provide view matrix and allows input to alter position
class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
    ~Camera();

    void            Update(GLfloat deltaTime);

    glm::mat4       getViewMatrix() const;
    glm::vec3       getPosition() const;
    glm::vec3       getFront() const;
    
    void            processKeyboard(Camera_Movement direction, GLfloat deltaTime);
    void            processMouse(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

private:

    void            updateCameraVectors();

    //Camera Attributes
    glm::vec3       m_position;
    glm::vec3       m_front;
    glm::vec3       m_up;
    glm::vec3       m_right;
    glm::vec3       m_worldUp;

    //Eular Angles
    GLfloat         m_yaw;
    GLfloat         m_pitch;

    //Camera Options
    GLfloat         m_movementSpeed;
    GLfloat         m_mouseSensitivity;
    GLfloat         m_zoom;

    


};
