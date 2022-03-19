#include "Input.h"

#include <iostream>

#include "EngineStatics.h"

#include "OpenGLWindow.h"

// Keyboard
bool Input::m_keys[1024];
int Input::m_lastKey = 0;

// Mouse
double Input::m_lastX = 0;
double Input::m_lastY = 0;
bool Input::m_firstMouse = true;
double Input::m_xOffset = 0;
double Input::m_yOffset = 0;

bool Input::m_mouseEnabled = false;

Input::Input()
{
	std::cout << "Input Initialized" << std::endl;
	 
	glfwSetKeyCallback(EngineStatics::getAppWindow()->getWindow(), keyCALLBACK);
	glfwSetCursorPosCallback(EngineStatics::getAppWindow()->getWindow(), mouseCALLBACK);

    glfwSetInputMode(EngineStatics::getAppWindow()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Input::~Input()
{
	std::cout << "Input Destroyed" << std::endl;
}

/// <summary>
/// Function is called every time application detects keyboard input
/// </summary>
/// <param name="window">The window that has input</param>
/// <param name="key">Key pressed</param>
void Input::keyCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
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

/// <summary>
/// Returns whether the specified key is pressed or not, but only once until the key is released
/// </summary>
/// <param name="key">Specified key to query (use GLFW_KEY_ macro)</param>
/// <returns></returns>
bool Input::getKeyPressedOnce(int key)
{
	if (key != m_lastKey && m_keys[key]) // Key is not same as last key and key was pressed
	{
		m_lastKey = key;
		return true;
	}
    return false;
}

/// <summary>
/// Function is called every time application detects mouse input
/// </summary>
/// <param name="window">The window that has input</param>
/// <param name="xPos">New mouse X position</param>
/// <param name="yPos">New mouse Y position</param>
void Input::mouseCALLBACK(GLFWwindow* window, double xPos, double yPos)
{
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

/// <summary>
/// Enables the mouse cursor and sets it to the centre of the window
/// </summary>
void Input::enableMouse()
{
    if (m_mouseEnabled) // If already enabled, leave function
        return;

    m_mouseEnabled = true;

	// Reset any lingering mouse movement
	m_lastX = 0;
	m_lastY = 0;
	m_xOffset = 0;
	m_yOffset = 0;

	glfwSetInputMode(EngineStatics::getAppWindow()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPos(EngineStatics::getAppWindow()->getWindow(), EngineStatics::getAppWindow()->getWindowWidth() / 2, EngineStatics::getAppWindow()->getWindowHeight() / 2);
}

/// <summary>
/// Disables the mouse cursor
/// </summary>
void Input::disableMouse()
{
    if (!m_mouseEnabled) // If already disabled, leave function
        return;

    m_mouseEnabled = false;

    glfwSetInputMode(EngineStatics::getAppWindow()->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/// <summary>
/// Returns true if cursor is enabled and false if not
/// </summary>
/// <returns></returns>
bool Input::getMouseEnabled()
{
	return m_mouseEnabled;
}
