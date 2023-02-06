#pragma once

enum class SceneName
{
	UNSET_SCENE = 0,
	FMPscene = 1,
	jamieTest = 2,
	lightTest = 3,
	materialTest = 4,
	shadowTest = 5
};

/// <summary>
/// Encapsulates all the objects of a loaded .xml scene file
/// Loops through the objects every frame updating and drawing them
/// </summary>
class Scene
{
public:
	Scene(const std::string& sceneFilename);
	~Scene();

	bool								LoadScene();

	void								UpdateScene();
	void								DrawScene();

	void								SetSceneCameraPosition(Vector3D newPosition);

	const std::string&					GetSceneName() { return m_sceneFilename; }
	std::weak_ptr<SceneLightManager>	GetSceneLightManager() { return m_sceneLightManager; }

	std::weak_ptr<Model>				GetModelAtIndex(unsigned int index);
	void								DeleteModelAtIndex(unsigned int index);
	size_t								GetNumberOfModels() { return m_sceneModels.size(); }


private:

	void								UpdateSceneLight();

	std::string							m_sceneFilename;

	std::shared_ptr<SceneCamera>		m_sceneCamera;
	std::shared_ptr<SceneLightManager>	m_sceneLightManager; 

	std::shared_ptr<SceneSky>			m_sceneSky;

	// All models used in scene
	SceneModels	m_sceneModels;
};
