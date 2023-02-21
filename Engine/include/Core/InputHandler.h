#pragma once

struct GLFWwindow;

/// <summary>
/// Observes input events from keyboard and mouse
/// Provides utility functions to check input for specific buttons
/// </summary>
class InputHandler
{
public:
	void			Init();
	static void		KEYCALLBACK(GLFWwindow* window, int key, int scancode, int action, int mode);

	// Returns whether the specified key is pressed or not
	bool			GetKeyPressed(int key) { return m_keys[key]; }
	bool			GetKeyPressedOnce(int key);

	static void		MOUSECALLBACK(GLFWwindow* window, double xPos, double yPos);
	void			GetMouseMoved(double& xMouse, double& yMouse);

	void			EnableMouse();
	void			DisableMouse();
	bool			GetMouseEnabled() { return m_mouseEnabled; }

	static InputHandler* Get()
	{
		static InputHandler* s_pInstance = new InputHandler;
		return s_pInstance;
	}

private:

	// Keyboard
	static bool		m_keys[1024];
	static uint16_t	m_lastKey;

	// Mouse
	static double	m_lastX;
	static double	m_lastY;
	static bool		m_firstMouse;
	static double	m_xOffset;
	static double	m_yOffset;

	static bool		m_mouseEnabled;

	InputHandler() {}
	~InputHandler() {}
};

/// <summary>
/// Replaces the GLFW_KEY_x from glfw3.h
/// </summary>
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
