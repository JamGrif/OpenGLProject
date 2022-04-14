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
//class SceneLightManager;
//class SceneCamera;
class Scene;
class LightingEntity;

// A wrapper around imGui to display UI elements to the window
class UI
{
public:
	UI(bool uiVisible, std::shared_ptr<Scene> loadedScene);
	~UI();

	void	startOfFrame();
	void	update();

	void	toggleUI();

	bool	getUiVisible() const;
	int		getSceneNum() const;
	int		getFilterNum() const;

	// Scene stuff
	void	updateSceneHandle(std::shared_ptr<Scene> newLoadedScene);

private:

	// Scene stuff
	void	sceneOptionsPanel();
	void	controlsPanel();
	void	performanceMetricsPanel();
	void	sceneEntitiesPanel();
	void	entityPanel();

	void	updateSceneInformation();

	void	clearSelectedEntity();

	bool			m_isEntitySelected;
	int				m_selectedEntityIndex;

	bool			m_uiVisible; // Dictates whether the UI will be drawn or not

	int				m_sceneNum; 

	bool			m_directionalLightInScene; // Indicates if the button to toggle the light should be visible or not
	bool			m_directionalLightActiveButton;

	bool			m_spotLightInScene;
	bool			m_spotLightActiveButton;

	bool			m_pointLightInScene[4];
	bool			m_pointLightActiveButton[4];

	enum
	{
		e_FirstPointLight	= 0,
		e_SecondPointLight	= 1,
		e_ThirdPointLight	= 2,
		e_FourthPointLight	= 3
	};

	int				m_appPostProcess;

	// Cached other classes
	std::shared_ptr<Scene>						m_sceneHandle;
};

