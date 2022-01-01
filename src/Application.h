#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\gtc\matrix_transform.hpp>

#include "OpenGLWindow.h"
#include "Input.h"
#include "Scene.h"



// Abstracts the program out of main, encapsulating the entire program
class Application
{
public:
	Application();
	~Application();

	bool		appInit();
	void		appLoop();

	static void windowResizeCALLBACK(GLFWwindow* window, int newWidth, int newHeight);

private:

	bool		changeScene(const std::string newSceneName);

	OpenGLWindow* m_appWindow;

	glm::mat4	m_projMatrix;

	GLuint		m_appVAO;

	// Objects

	Input*		m_input;
	
	Scene*		m_loadedScene;

};

