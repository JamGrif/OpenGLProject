#pragma once

struct GLFWwindow;

// Static functions allows the checking of key presses anywhere in the program
class InputHandler
{
public:
	void		init();
	static void	keyCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode);

	bool		getKeyPressed(int key);
	bool		getKeyPressedOnce(int key);

	static void	mouseCALLBACK(GLFWwindow* window, double xPos, double yPos);
	void		getMouseMoved(double& xMouse, double& yMouse);

	void		enableMouse();
	void		disableMouse();
	bool		getMouseEnabled();

	static InputHandler* Instance()
	{
		static InputHandler* s_pInstance = new InputHandler;
		return s_pInstance;
	}

private:

	// Keyboard
	static bool	m_keys[1024];
	static int		m_lastKey;

	// Mouse
	static double	m_lastX;
	static double	m_lastY;
	static bool	m_firstMouse;
	static double	m_xOffset;
	static double	m_yOffset;

	static bool		m_mouseEnabled;

	InputHandler() {}
};

// Replaces the GLFW_KEY_x from glfw3.h
namespace Keyboard
{
	enum Keys
	{
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
	};
}
