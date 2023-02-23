#pragma once

enum class SceneName;
class UI;
class Scene;


bool StartEngine();


/// <summary>
/// Encapsulates the entire engine
/// All parts of the engine are initialized, looped through and cleaned from this class
/// </summary>
class Engine
{
private:
	Engine();
	~Engine();


	bool					Initialise();
	void					Loop();

	void					HandleInput();
	void					UpdateApp();
	void					RenderApp();

	bool					SetScene(SceneName newSceneNumber);

	UI*		m_UI;

	Scene*	m_loadedScene;

	friend bool StartEngine();
};
