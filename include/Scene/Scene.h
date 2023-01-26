#pragma once

class Model;

class SceneCamera;
class SceneLightManager;
class SceneSky;

typedef std::vector<std::unique_ptr<Model>> SceneModels;

// Class that contains all the objects of a loaded level. Loops through them every frame, updating and drawing them
class Scene
{
public:
	Scene(const std::string& sceneFilename);
	~Scene();

	bool								LoadScene();

	void								UpdateScene();
	void								DrawScene();

	const std::string&					GetSceneName() { return m_sceneFilename; }
	const std::shared_ptr<SceneLightManager>& GetSceneLightManager() { return m_sceneLightManager; }

private:

	void								UpdateSceneLight();

	std::string							m_sceneFilename;

	std::shared_ptr<SceneCamera>		m_sceneCamera;
	std::shared_ptr<SceneLightManager>	m_sceneLightManager; 

	std::shared_ptr<SceneSky>			m_sceneSky;

	// All models used in scene
	SceneModels	m_sceneModels;
};
