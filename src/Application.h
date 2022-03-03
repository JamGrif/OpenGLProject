#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\gtc\matrix_transform.hpp>

#include <memory>

#include "OpenGLWindow.h"
#include "Input.h"
#include "Scene.h"
#include "UI.h"

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

	void		checkForSceneChange();
	bool		changeScene(const std::string& newSceneName);

	OpenGLWindow* m_appWindow;

	glm::mat4	m_projMatrix;

	GLuint		m_appVAO;

	// Objects

	Input*		m_input;

	UI*			m_UI;
	
	Scene*		m_loadedScene;

};

