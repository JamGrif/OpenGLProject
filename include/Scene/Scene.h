#pragma once

// Forward Declarations
class BaseEntity;
class LightingEntity;
class SceneCamera;
class SceneLightManager;

// Class that contains all the objects of a loaded level. Loops through them every frame, updating and drawing them
class Scene
{
public:
	Scene(const std::string& sceneName);
	~Scene();

	bool						loadScene();

	void						updateScene();

	void						drawSceneFirstPass();
	void						drawSceneSecondPass();

	const std::string&			getSceneName();

	size_t							getEntityNum();
	std::shared_ptr<LightingEntity>	getEntityAtIndex(int index);


	const std::shared_ptr<SceneLightManager>&	getSceneLightManager();
	const std::shared_ptr<SceneCamera>&			getSceneCamera();

	void					deleteLightingEntityFromVector(int index);

private:

	void					addSceneCamera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void					addSceneLightManager();

	void					updateSceneLight();


	const std::string							m_sceneName;

	std::shared_ptr<SceneCamera>				m_sceneCamera;
	std::shared_ptr<SceneLightManager>			m_sceneLightManager;

	std::vector<std::shared_ptr<BaseEntity>>	m_sceneEntities;

	// A separate vector to store just the LightingEntity objects as it makes it easier for the UI (will change to a better system eventually)
	std::vector<std::shared_ptr<LightingEntity>> m_sceneLightingEntities;
};
