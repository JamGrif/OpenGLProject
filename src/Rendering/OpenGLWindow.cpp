#include "pch.h"
#include "Rendering/OpenGLWindow.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h"

#include <GLFW/glfw3.h>

bool OpenGLWindow::Init(int windowWidth, int windowHeight, const std::string& windowTitle, bool fullScreen)
{
	m_currentWindowWidth = windowWidth;
	m_currentWindowHeight = windowHeight;

	// Window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, NULL); // Borderless window

	if (fullScreen)
	{
		m_window = glfwCreateWindow(m_currentWindowWidth, m_currentWindowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		m_window = glfwCreateWindow(m_currentWindowWidth, m_currentWindowHeight, windowTitle.c_str(), NULL, NULL);
	}

	// Check if window was made successfully
	if (!m_window)
	{
		return false;
	}

	// Set Icon
	SetWindowIcon("res/icon/Icon.jpg");

	glfwMakeContextCurrent(m_window);

	glfwSwapInterval(1); // Double buffering
	glClearColor(0.0, 0.0, 0.0, 1.0); // Sets clear colour

	glViewport(0, 0, m_currentWindowWidth, m_currentWindowHeight);
	m_aspectRatio = (float)m_currentWindowWidth / (float)m_currentWindowHeight;

	return true;
}

bool OpenGLWindow::Clean()
{
	glfwDestroyWindow(m_window);

	return true;
}

/// <summary>
/// Returns the pointer to the actual GLFWwindow variable itself
/// </summary>
GLFWwindow* OpenGLWindow::GetWindowPtr() const
{
	return m_window;
}

bool OpenGLWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

/// <summary>
/// Sets this window to the specified picture at iconPath
/// </summary>
void OpenGLWindow::SetWindowIcon(const std::string& iconPath) const
{
	GLFWimage image;
	image.pixels = stbi_load(iconPath.c_str(), &image.width, &image.height, 0, 4); // RGBA channels 
	glfwSetWindowIcon(m_window, 1, &image);
	stbi_image_free(image.pixels);
}

/// <summary>
/// Sets the window title to the specified string text
/// </summary>
void OpenGLWindow::SetWindowTitle(const std::string& newTitle) const
{
	glfwSetWindowTitle(m_window, newTitle.c_str());
}

/// <summary>
/// Returns the windows width
/// </summary>
int OpenGLWindow::GetWindowWidth() const
{
	return m_currentWindowWidth;
}

/// <summary>
/// Returns the windows height
/// </summary>
int OpenGLWindow::GetWindowHeight() const
{
	return m_currentWindowHeight;
}

/// <summary>
/// Returns the aspect ratio of the window
/// </summary>
float OpenGLWindow::GetAspectRatio() const
{
	return m_aspectRatio;
}

