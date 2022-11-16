#pragma once


// Forward Declarations
class Scene;
class UI;
class OpenGLFramebuffer;
class OpenGLRenderer;

// Abstracts the program out of main.cpp, encapsulating the entire program
class Application
{
public:
	Application();
	~Application();

	bool	appInit();
	void	appLoop();

private:

	bool	setScene(int newSceneNumber);
	void	setScreenFilter(int newFilterNumber);

	// Objects

	//std::shared_ptr<OpenGLRenderer>			m_renderer;

	std::unique_ptr<UI>					m_UI;

	std::shared_ptr<Scene>				m_loadedScene;

	std::unique_ptr<OpenGLFramebuffer>	m_sceneMSAAFrameBuffer;		//Scene is drawn to this buffer with MSAA applied
	std::unique_ptr<OpenGLFramebuffer>	m_sceneFilterFramebuffer;	//Recieves info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen
};

