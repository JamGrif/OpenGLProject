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
class GameTimer;

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

	std::shared_ptr<OpenGLWindow>	m_appWindow;

	glm::mat4						m_projMatrix;

	GLuint							m_appVAO;

	// Objects

	std::unique_ptr<Input>			m_input;

	std::unique_ptr<UI>				m_UI;

	std::unique_ptr<Scene>			m_loadedScene;

	std::unique_ptr<Framebuffer>	m_sceneMSAAFrameBuffer;		//Scene is drawn to this buffer with MSAA applied
	std::unique_ptr<Framebuffer>	m_sceneFilterFramebuffer;	//Recieves info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen

	std::shared_ptr<GameTimer>		m_gameTimer;
};

