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
class Framebuffer;

// Abstracts the program out of main.cpp, encapsulating the entire program
class Application
{
public:
	Application();
	~Application();

	bool			appInit();
	void			appLoop();

	static void		windowResizeCALLBACK(GLFWwindow* window, int newWidth, int newHeight);

private:

	bool			changeScene(int newSceneNumber);
	void			changeScreenFilter(int newFilterNumber);

	OpenGLWindow*	m_appWindow;

	glm::mat4		m_projMatrix;

	GLuint			m_appVAO;

	// Cached screen size
	int				m_cachedScreenWidth;
	int				m_cachedScreenHeight;

	// Objects

	Input*			m_input;

	UI*				m_UI;

	Scene*			m_loadedScene;

	Framebuffer*	m_sceneMSAAFrameBuffer;		//Scene is drawn to this buffer with MSAA applied
	Framebuffer*	m_sceneFilterFramebuffer;	//Recieves info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen

};

