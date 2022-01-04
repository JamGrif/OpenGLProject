#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

/// <summary>
/// A wrapper class around the GLFWwindow variable, providing a way to create and destroy one, 
/// along with utility functions and a way to check its status.
/// </summary>
class OpenGLWindow
{
public:
	OpenGLWindow(int windowWidth, int windowHeight, std::string windowTitle, bool fullScreen);
	~OpenGLWindow();

	GLFWwindow* getWindow();

	void		setWindowIcon(const std::string& iconPath);
	void		setWindowTitle(const std::string& newTitle);

	int			getWindowWidth();
	int			getWindowHeight();
	float		getAspectRatio();
	bool		getWindowStatus();

private:

	bool		m_status;

	int			m_currentWindowWidth;
	int			m_currentWindowHeight;
	float		m_aspectRatio;

	GLFWwindow* m_Window;

};
