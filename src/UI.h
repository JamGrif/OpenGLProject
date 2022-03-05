#pragma once

#include "LightManager.h"

enum sceneNames
{
	e_FMPscene		= 1,
	e_jamieTest		= 2,
	e_lightTest		= 3,
	e_materialTest	= 4,
	e_shadowTest	= 5
};

// A wrapper around imGui to display UI elements to the window
class UI
{
public:
	UI(bool uiVisible);
	~UI();

	void	startOfFrame();
	void	drawInFrame();

	void	toggleUI();

	bool	getUiVisible() const;
	int		getSceneNum() const;
	int		getFilterNum() const;

	void	refreshLightButtons();

private:

	bool	m_uiVisible;

	int		m_sceneNum;

	bool	m_directionalLightInScene;
	bool	m_directionalLightActive;

	bool	m_spotLightInScene;
	bool	m_spotLightActive;

	bool	m_pointLightInScene[4];
	bool	m_pointLightActive[4];

	enum
	{
		e_FirstPointLight	= 0,
		e_SecondPointLight	= 1,
		e_ThirdPointLight	= 2,
		e_FourthPointLight	= 3
	};

	int m_appPostProcess;

	//Cached other classes
	LightManager* m_localLightManager;
};

