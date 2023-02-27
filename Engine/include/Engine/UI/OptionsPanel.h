#pragma once
#include "UI/IPanel.h"

/// <summary>
/// Displays buttons and checkboxes to manipulate the current scene
/// </summary>
class OptionsPanel :
    public IPanel
{
public:
	OptionsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer);
	~OptionsPanel();

	virtual void	Update() override;
	virtual void	Render() override;
	virtual void	SceneChange() override;

private:

	// Indicates if the button to toggle the light should be visible or not
	bool			m_bDirectionalLightInScene;
	bool			m_bSpotLightInScene;
	bool			m_bPointLightInScene[4];

	// Toggle state of each light button (if active)
	bool			m_bDirectionalLightActiveButton;
	bool			m_bSpotLightActiveButton;
	bool			m_bPointLightActiveButton[4];

	uint8_t			m_totalPointLights;
};
