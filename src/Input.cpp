#include "Input.h"

#include <iostream>

#include "EngineStatics.h"

//Keyboard
bool Input::m_keys[1024];
int Input::m_lastKey = 0;

//Mouse
double Input::m_lastX = 0;
double Input::m_lastY = 0;
bool Input::m_firstMouse = true;
double Input::m_xOffset = 0;
double Input::m_yOffset = 0;


Input::Input()
{
	std::cout << "Input Initialized" << std::endl;
	 
	glfwSetKeyCallback(EngineStatics::getAppWindow(), keyCALLBACK);
	glfwSetCursorPosCallback(EngineStatics::getAppWindow(), mouseCALLBACK);

    glfwSetInputMode(EngineStatics::getAppWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Hide cursor during runtime

}

Input::~Input()
{
	std::cout << "Input Destroyed" << std::endl;
}

/// <summary>
/// Function is called everytime application detects keyboard input
/// </summary>
/// <param name="window">The window that has input</param>
/// <param name="key">Key pressed</param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mode"></param>
void Input::keyCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
           // lastKey = key;
            m_keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_lastKey = 0;
            m_keys[key] = false;
        }
    }
}

/// <summary>
/// Returns whether the specified key is pressed or not
/// </summary>
/// <param name="key">Specified key to query</param>
/// <returns>If the key is pressed or not</returns>
bool Input::getKeyPressed(int key)
{
    if (m_keys[key])
    {
        
        return true;
    }
	return false;
}

bool Input::getKeyPressedOnce(int key)
{
    if (m_keys[key] && key != m_lastKey)
    {
        m_lastKey = key;
        return true;
    }
    return false;
}

/// <summary>
/// Function is called everytime application detects mouse input
/// </summary>
/// <param name="window">The window that has input</param>
/// <param name="xPos">New mouse X position</param>
/// <param name="yPos">New mouse Y position</param>
void Input::mouseCALLBACK(GLFWwindow* window, double xPos, double yPos)
{
    //std::cout << "mousecallback" << std::endl;
    if (m_firstMouse)
    {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouse = false;
    }
    m_xOffset = xPos - m_lastX;
    m_yOffset = m_lastY - yPos; //Reversed as Y coordinates go from bottom to left

    m_lastX = xPos;
    m_lastY = yPos;
}

/// <summary>
/// Returns how much the mouse has moved since last function call
/// </summary>
/// <param name="xMouse"></param>
/// <param name="yMouse"></param>
void Input::getMouseMoved(double& xMouse, double& yMouse)
{
    xMouse = m_xOffset;
    yMouse = m_yOffset;

    m_xOffset = 0;
    m_yOffset = 0;

}

