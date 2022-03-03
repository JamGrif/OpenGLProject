#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\gtc\matrix_transform.hpp>

#include <memory>
#include <string>

// Forward Declarations
class OpenGLWindow;
class Input;
class Scene;
class UI;

// Abstracts the program out of main.cpp, encapsulating the entire program
class Application
{
public:
	Application();
	~Application();

	bool		appInit();
	void		appLoop();

	static void windowResizeCALLBACK(GLFWwindow* window, int newWidth, int newHeight);

private:

	bool		changeScene(int newSceneNumber);

	OpenGLWindow* m_appWindow;

	glm::mat4	m_projMatrix;

	GLuint		m_appVAO;

	// Objects

	Input*		m_input;

	UI*			m_UI;
	
	Scene*		m_loadedScene;

};

