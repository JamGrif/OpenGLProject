#pragma once

enum class SceneName
{
	UNSET_SCENE		= 0,
	FMPscene		= 1,
	jamieTest		= 2,
	lightTest		= 3,
	materialTest	= 4,
	shadowTest		= 5
};

// Forward Declarations
class Scene;
class Model;

// A wrapper around imGui to display UI elements to the window
class UI
{
public:
	UI(bool uiVisible);
	~UI();

	void		RenderUI();

	void		ToggleUI();

	bool		GetUiVisible() const { return m_uiVisible; }
	SceneName	GetCurrentSceneName() const { return m_currentSceneName; }
	int			GetFilterNum() const { return m_appPostProcess; }

	void		UpdateSceneHandle(std::weak_ptr<Scene> newLoadedScene);

private:

	// Scene stuff
	void		SceneOptionsPanel();
	void		ControlsPanel();
	void		PerformanceMetricsPanel();

	void		UpdateSceneInformation();

	bool		m_uiVisible;

	SceneName	m_currentSceneName; 

	bool		m_bDirectionalLightInScene; // Indicates if the button to toggle the light should be visible or not
	bool		m_bDirectionalLightActiveButton;

	bool		m_bSpotLightInScene;
	bool		m_bSpotLightActiveButton;

	int			m_totalPointLights;
	bool		m_bPointLightInScene[4];
	bool		m_bPointLightActiveButton[4];

	int			m_appPostProcess;

	// Cached scene handle
	std::weak_ptr<Scene>	m_sceneHandle;
};

