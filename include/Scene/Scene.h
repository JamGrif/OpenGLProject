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

	bool							loadScene();

	void							updateScene();
	void							drawScene();

	const std::string& getSceneName() { return m_sceneName; }

	const std::shared_ptr<SceneLightManager>& getSceneLightManager() { return m_sceneLightManager; }

private:

	void							addSceneCamera(Vector3D position);
	void							addSceneLightManager();

	void							updateSceneLight();


	const std::string&					m_sceneName;

	std::shared_ptr<SceneCamera>		m_sceneCamera;
	std::shared_ptr<SceneLightManager>	m_sceneLightManager; 

	SceneSky*							m_sceneSky;

	// A separate vector to store just the LightingEntity objects as it makes it easier for the UI (will change to a better system eventually)
	std::vector<std::shared_ptr<Model>>	m_sceneModels;
};
