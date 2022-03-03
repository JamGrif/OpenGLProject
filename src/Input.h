#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Static functions allows the checking of key presses anywhere in the program
class Input
{
public:
	Input();
	~Input();

	static void		keyCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode);
	static bool		getKeyPressed(int key);
	static bool		getKeyPressedOnce(int key);

	static void		mouseCALLBACK(GLFWwindow* window, double xPos, double yPos);
	static void		getMouseMoved(double& xMouse, double& yMouse);

	static bool		getMouseEnabled();
	static void		enableMouse();
	static void		disableMouse();

private:

	// Keyboard
	static bool		m_keys[1024];
	static int		m_lastKey;

	// Mouse
	static double	m_lastX;
	static double	m_lastY;
	static bool		m_firstMouse;
	static double	m_xOffset;
	static double	m_yOffset;

	static bool		m_mouseEnabled;
};

