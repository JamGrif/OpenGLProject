#pragma once

class Model;

class SceneCamera;
class SceneLightManager;
class SceneSky;

// Class that contains all the objects of a loaded level. Loops through them every frame, updating and drawing them
class Scene
{
public:
	Scene(const std::string& sceneName);
	~Scene();

	bool								LoadScene();

	void								UpdateScene();
	void								DrawScene();

	const std::string&					GetSceneName() { return m_sceneName; }
	const std::shared_ptr<SceneLightManager>& GetSceneLightManager() { return m_sceneLightManager; }

private:

	void								UpdateSceneLight();

	std::string							m_sceneName;

	std::shared_ptr<SceneCamera>		m_sceneCamera;
	std::shared_ptr<SceneLightManager>	m_sceneLightManager; 

	std::shared_ptr<SceneSky>			m_sceneSky;

	// All models used in scene
	std::vector<std::shared_ptr<Model>>	m_sceneModels;
};
