#pragma once

class UI;
class Scene;

// Abstracts the program out of main.cpp, encapsulating the entire program
class Application
{
public:
	Application();
	~Application();

	bool	appInit();
	void	appLoop();

	void	handleInput();
	void	updateApp();
	void	renderApp();

private:

	bool	setScene(int newSceneNumber);

	std::unique_ptr<UI>					m_UI;

	std::shared_ptr<Scene>				m_loadedScene;
};

