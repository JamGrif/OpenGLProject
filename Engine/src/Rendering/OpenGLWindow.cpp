#include "pch.h"
#include "Rendering/OpenGLWindow.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h"

#include "GLFW/glfw3.h"

static constexpr int OPENGL_MAJOR_VERSION = 4;
static constexpr int OPENGL_MINOR_VERSION = 6;

/// <summary>
/// Set GLFW window hints, create the window and set OpenGL context and viewport
/// </summary>
bool OpenGLWindow::Init(int windowWidth, int windowHeight, const std::string& windowTitle, bool bFullScreen)
{
	// Setup window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, NULL); // Borderless window

	if (bFullScreen)
		m_pWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
	else
		m_pWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
	
	if (!m_pWindow)
		return false;

	glfwMakeContextCurrent(m_pWindow);

	m_currentWindowWidth = windowWidth;
	m_currentWindowHeight = windowHeight;

	glViewport(NULL, NULL, m_currentWindowWidth, m_currentWindowHeight);
	m_aspectRatio = (float)m_currentWindowWidth / (float)m_currentWindowHeight;

	// Set initial OpenGL window values
	glfwSwapInterval(true); // Double buffering
	glClearColor(0.0, 0.0, 0.0, 1.0); // Sets clear colour

	return true;
}

/// <summary>
/// Destroy GLFW window object
/// </summary>
bool OpenGLWindow::Clean()
{
	glfwDestroyWindow(m_pWindow);

	return true;
}

/// <summary>
/// Returns whether the GLFW window has received a close event
/// </summary>
bool OpenGLWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_pWindow);
}

/// <summary>
/// Sets this window to the specified picture at iconPath
/// </summary>
void OpenGLWindow::SetWindowIcon(const std::string& iconPath) const
{
	GLFWimage image;
	image.pixels = stbi_load(iconPath.c_str(), &image.width, &image.height, 0, 4); // RGBA channels 
	glfwSetWindowIcon(m_pWindow, 1, &image);
	stbi_image_free(image.pixels);
}

/// <summary>
/// Sets the window title to the specified string
/// </summary>
void OpenGLWindow::SetWindowTitle(const std::string& newTitle) const
{
	glfwSetWindowTitle(m_pWindow, newTitle.c_str());
}
