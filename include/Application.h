#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\gtc\matrix_transform.hpp>

// Forward Declarations
class OpenGLWindow;
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

	bool			setScene(int newSceneNumber);
	void			setScreenFilter(int newFilterNumber);

	std::shared_ptr<OpenGLWindow>	m_appWindow;

	glm::mat4						m_projMatrix;

	GLuint							m_appVAO;

	// Objects

	std::unique_ptr<UI>				m_UI;

	std::unique_ptr<Scene>			m_loadedScene;

	std::unique_ptr<Framebuffer>	m_sceneMSAAFrameBuffer;		//Scene is drawn to this buffer with MSAA applied
	std::unique_ptr<Framebuffer>	m_sceneFilterFramebuffer;	//Recieves info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen
};

