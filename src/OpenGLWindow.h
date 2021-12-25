#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class OpenGLWindow
{
public:
	OpenGLWindow(int windowWidth, int windowHeight, std::string windowTitle, bool fullScreen);
	~OpenGLWindow();

	bool getWindowStatus();
	GLFWwindow* getWindow();

	void setWindowIcon(const std::string& iconPath);
	void setWindowTitle(const std::string& newTitle);

	int		getWindowWidth();
	int		getWindowHeight();
	float	getAspectRatio();

private:

	bool		m_status;

	int			m_currentWindowWidth;
	int			m_currentWindowHeight;
	float		m_aspectRatio;

	GLFWwindow* m_Window;

};
