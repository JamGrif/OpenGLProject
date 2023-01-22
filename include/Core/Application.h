#pragma once

class UI;
class Scene;

// Abstracts the program out of main.cpp, encapsulating the entire program
class Application
{
public:
	Application();
	~Application();

	bool	AppInit();
	void	AppLoop();

	void	HandleInput();
	void	UpdateApp();
	void	RenderApp();

private:

	bool	SetScene(int newSceneNumber);

	std::unique_ptr<UI>					m_UI;

	std::shared_ptr<Scene>				m_loadedScene;
};

