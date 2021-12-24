#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\gtc\matrix_transform.hpp>

#include "Input.h"
#include "Scene.h"

//Abstracts the program out of main, encapsulating the entire program
class Application
{
public:
	Application();
	~Application();

	int			appInit();
	void		appLoop();

	static void windowResizeCALLBACK(GLFWwindow* window, int newWidth, int newHeight);

private:

	bool		changeScene(const std::string newSceneName);

	void		calculateDeltaTime();

	//Window
	const int	m_defaultWindowWidth;
	const int	m_defaultWindowHeight;
	int			m_currentWindowWidth;
	int			m_currentWindowHeight;

	float		m_aspectRatio;

	// OpenGL
	GLFWwindow* m_appWindow;

	glm::mat4	m_projMatrix;

	GLuint		m_appVAO;

	// Objects

	Scene*		m_loadedScene;

	Input*		m_input;

	// Delta time
	GLfloat		m_deltaTime;
	GLfloat		m_lastFrame;

	double		m_previousTime;
	int			m_frameCount;
	double		m_currentFrame;
};

