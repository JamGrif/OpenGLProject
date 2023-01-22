#pragma once

enum sceneNames
{
	e_FMPscene		= 1,
	e_jamieTest		= 2,
	e_lightTest		= 3,
	e_materialTest	= 4,
	e_shadowTest	= 5
};

// Forward Declarations
class Scene;
class Model;

// A wrapper around imGui to display UI elements to the window
class UI
{
public:
	UI(bool uiVisible, std::shared_ptr<Scene> loadedScene);
	~UI();

	void	RenderUI();

	void	ToggleUI();

	bool	GetUiVisible() const { return m_uiVisible; }
	int		GetSceneNum() const { return m_sceneNum; }
	int		GetFilterNum() const { return m_appPostProcess; }

	void	UpdateSceneHandle(std::shared_ptr<Scene> newLoadedScene);

private:

	// Scene stuff
	void	SceneOptionsPanel();
	void	ControlsPanel();
	void	PerformanceMetricsPanel();

	void	UpdateSceneInformation();

	bool	m_uiVisible;

	int		m_sceneNum; 

	bool	m_directionalLightInScene; // Indicates if the button to toggle the light should be visible or not
	bool	m_directionalLightActiveButton;

	bool	m_spotLightInScene;
	bool	m_spotLightActiveButton;

	int		m_totalPointLights;
	bool	m_pointLightInScene[4];
	bool	m_pointLightActiveButton[4];

	int		m_appPostProcess;

	// Cached scene handle
	std::shared_ptr<Scene>					m_sceneHandle;
};

