#include "pch.h"
#include "Core/InputHandler.h"

#include "Rendering/OpenGLWindow.h"

#include <GLFW/glfw3.h>

// Keyboard
bool	InputHandler::m_keys[1024];
int		InputHandler::m_lastKey = 0;

// Mouse
double	InputHandler::m_lastX = 0;
double	InputHandler::m_lastY = 0;
bool	InputHandler::m_firstMouse = true;
double	InputHandler::m_xOffset = 0;
double	InputHandler::m_yOffset = 0;

bool	InputHandler::m_mouseEnabled = false;

/// <summary>
/// Initializes the input of the program by setting up the input callbacks
/// </summary>
void InputHandler::init()
{
	glfwSetKeyCallback(TheOpenGLWindow::Instance()->getWindowPtr(), keyCALLBACK);
	glfwSetCursorPosCallback(TheOpenGLWindow::Instance()->getWindowPtr(), mouseCALLBACK);

	glfwSetInputMode(TheOpenGLWindow::Instance()->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/// <summary>
/// Function is called every time application detects keyboard input
/// </summary>
void InputHandler::keyCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode)
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
bool InputHandler::getKeyPressed(int key)
{
	return m_keys[key];
}

/// <summary>
/// Returns whether the specified key is pressed or not, but only once until the key is released
/// Use macro in Input.h
/// </summary>
bool InputHandler::getKeyPressedOnce(int key)
{
	if (key != m_lastKey && m_keys[key]) // Key is not same as last key and key was pressed
	{
		m_lastKey = key;
		return true;
	}
    return false;
}

/// <summary>
/// Function is called every time application detects mouse input on specified window
/// </summary>
void InputHandler::mouseCALLBACK(GLFWwindow* window, double xPos, double yPos)
{
	if (m_firstMouse)
	{
		m_lastX = xPos;
		m_lastY = yPos;
		m_firstMouse = false;
	}

    m_xOffset = xPos - m_lastX;
    m_yOffset = m_lastY - yPos; //Reversed as Y coordinates go from bottom to top

    m_lastX = xPos;
    m_lastY = yPos;
}

/// <summary>
/// Returns how much the mouse has moved since last function call
/// </summary>
void InputHandler::getMouseMoved(double& xMouse, double& yMouse)
{
	xMouse = m_xOffset;
	yMouse = m_yOffset;

	m_xOffset = 0;
	m_yOffset = 0;
}

/// <summary>
/// Enables the mouse cursor and sets it to the centre of the window
/// </summary>
void InputHandler::enableMouse()
{
    if (m_mouseEnabled) // If already enabled, leave function
        return;

    m_mouseEnabled = true;

	glfwSetInputMode(TheOpenGLWindow::Instance()->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPos(TheOpenGLWindow::Instance()->getWindowPtr(), TheOpenGLWindow::Instance()->getWindowWidth() / 2, TheOpenGLWindow::Instance()->getWindowHeight() / 2);

	// Reset any lingering mouse movement
	m_xOffset = 0;
	m_yOffset = 0;
}

/// <summary>
/// Disables the mouse cursor
/// </summary>
void InputHandler::disableMouse()
{
    if (!m_mouseEnabled) // If already disabled, leave function
        return;

    m_mouseEnabled = false;

	PRINT_TRACE("hi");

	PRINT_TRACE("{0}", m_lastX);
	PRINT_TRACE("{0}", m_lastY);
	PRINT_TRACE("{0}", m_xOffset);
	PRINT_TRACE("{0}", m_xOffset);

	// Reset any lingering mouse movement
	m_xOffset = 0;
	m_yOffset = 0;

    glfwSetInputMode(TheOpenGLWindow::Instance()->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/// <summary>
/// Returns true if cursor is enabled and false if not
/// </summary>
bool InputHandler::getMouseEnabled()
{
	return m_mouseEnabled;
}
