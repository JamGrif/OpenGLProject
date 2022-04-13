#include "pch.h"
#include "Rendering/OpenGLWindow.h"

#include "Core/EngineStatics.h"
#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h"

#include <GLFW/glfw3.h>

OpenGLWindow::OpenGLWindow(int windowWidth, int windowHeight, const std::string& windowTitle, bool fullScreen)
	:m_status(false), m_currentWindowWidth(windowWidth), m_currentWindowHeight(windowHeight), m_aspectRatio(0), m_Window(nullptr)
{
	// Window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, NULL); // Borderless window

	if (fullScreen)
	{
		m_Window = glfwCreateWindow(m_currentWindowWidth, m_currentWindowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		m_Window = glfwCreateWindow(m_currentWindowWidth, m_currentWindowHeight, windowTitle.c_str(), NULL, NULL);
	}
	
	// Check if window was made successfully
	if (!m_Window)
	{
		m_status = false;
		return;
	}

	// Set Icon
	setWindowIcon("res/icon/Icon.jpg");
	
	glfwMakeContextCurrent(m_Window);

	// Set EngineStatics values
	//EngineStatics::setAppWindow(this);

	glfwSwapInterval(1); // Double buffering
	glClearColor(0.0, 0.0, 0.0, 1.0); // Sets clear colour

	//glfwGetFramebufferSize(m_Window, &m_currentWindowWidth, &m_currentWindowHeight);

	glViewport(0, 0, m_currentWindowWidth, m_currentWindowHeight);
	m_aspectRatio = (float)m_currentWindowWidth / (float)m_currentWindowHeight;

	m_status = true;
}

OpenGLWindow::~OpenGLWindow()
{
	// Clear EngineStatics values
	EngineStatics::setAppWindow(nullptr);

	glfwDestroyWindow(m_Window);
}

/// <summary>
/// Returns the pointer to the actual GLFWwindow variable itself
/// </summary>
GLFWwindow* OpenGLWindow::getRaw() const
{
	return m_Window;
}

bool OpenGLWindow::shouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

/// <summary>
/// Sets this window to the specified picture at iconPath
/// </summary>
void OpenGLWindow::setWindowIcon(const std::string& iconPath) const
{
	GLFWimage images[1];
	images[0].pixels = stbi_load(iconPath.c_str(), &images[0].width, &images[0].height, 0, 4); // RGBA channels 
	glfwSetWindowIcon(m_Window, 1, images);
	stbi_image_free(images[0].pixels);
}

/// <summary>
/// Sets the window title to the specified string text
/// </summary>
void OpenGLWindow::setWindowTitle(const std::string& newTitle) const
{
	glfwSetWindowTitle(m_Window, newTitle.c_str());
}

/// <summary>
/// Returns the windows width
/// </summary>
int OpenGLWindow::getWindowWidth() const
{
	return m_currentWindowWidth;
}

/// <summary>
/// Returns the windows height
/// </summary>
int OpenGLWindow::getWindowHeight() const
{
	return m_currentWindowHeight;
}

/// <summary>
/// Returns the aspect ratio of the window
/// </summary>
float OpenGLWindow::getAspectRatio() const
{
	return m_aspectRatio;
}

/// <summary>
/// Returns the status of the window
/// </summary>
/// <returns>"true" if window is alright, "false" if window is in an error state and should not be used</returns>
bool OpenGLWindow::getWindowStatus() const
{
	return m_status;
}
