#include "OpenGLWindow.h"

#include "stb_image.h"
#include "EngineStatics.h"
#include <iostream>

OpenGLWindow::OpenGLWindow(int windowWidth, int windowHeight, std::string windowTitle, bool fullScreen)
	:m_status(false), m_currentWindowWidth(windowWidth), m_currentWindowHeight(windowHeight), m_aspectRatio(0), m_Window(nullptr)
{
	// Window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_DECORATED, NULL); // borderless window

	if (fullScreen)
	{
		m_Window = glfwCreateWindow(m_currentWindowWidth, m_currentWindowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else
	{

		m_Window = glfwCreateWindow(m_currentWindowWidth, m_currentWindowHeight, windowTitle.c_str(), NULL, NULL);
	}
	
	if (m_Window == nullptr)
	{
		m_status = false;
		return;
	}

	// Set Icon
	setWindowIcon("res/icon/Icon.jpg");
	
	glfwMakeContextCurrent(m_Window);
	EngineStatics::setAppWindow(m_Window);
	EngineStatics::setScreenWidth(m_currentWindowWidth);
	EngineStatics::setScreenHeight(m_currentWindowHeight);

	glfwSwapInterval(1);
	glClearColor(0.0, 0.0, 0.0, 1.0); //Sets clear colour

	glfwGetFramebufferSize(m_Window, &m_currentWindowWidth, &m_currentWindowHeight);

	glViewport(0, 0, m_currentWindowWidth, m_currentWindowHeight);
	m_aspectRatio = (float)m_currentWindowWidth / (float)m_currentWindowHeight;

	m_status = true;
}

OpenGLWindow::~OpenGLWindow()
{
	EngineStatics::setAppWindow(nullptr);
	EngineStatics::setScreenWidth(0);
	EngineStatics::setScreenHeight(0);
	glfwDestroyWindow(m_Window);
}

bool OpenGLWindow::getWindowStatus()
{
	return m_status;
}

GLFWwindow* OpenGLWindow::getWindow()
{
	return m_Window;
}

void OpenGLWindow::setWindowIcon(const std::string& iconPath)
{
	GLFWimage images[1];
	images[0].pixels = stbi_load(iconPath.c_str(), &images[0].width, &images[0].height, 0, 4); // RGBA channels 
	glfwSetWindowIcon(m_Window, 1, images);
	stbi_image_free(images[0].pixels);
}

void OpenGLWindow::setWindowTitle(const std::string& newTitle)
{
	glfwSetWindowTitle(m_Window, newTitle.c_str());
}

int OpenGLWindow::getWindowWidth()
{
	return m_currentWindowWidth;
}

int OpenGLWindow::getWindowHeight()
{
	return m_currentWindowHeight;
}

float OpenGLWindow::getAspectRatio()
{
	return m_aspectRatio;
}
