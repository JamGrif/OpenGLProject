#pragma once
#include "UI/IPanel.h"

enum class SceneName;

class OptionsPanel :
    public IPanel
{
public:
	OptionsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible);
	~OptionsPanel();

	virtual void Update() override;
	virtual void Render() override;
	virtual void SceneChange() override;

	SceneName GetSelectedSceneName() { return m_selectedSceneName; }

private:

	SceneName	m_selectedSceneName;

	// Indicates if the button to toggle the light should be visible or not
	bool		m_bDirectionalLightInScene;
	bool		m_bSpotLightInScene;
	bool		m_bPointLightInScene[4];

	// Toggle state of each light button (if active)
	bool		m_bDirectionalLightActiveButton;
	bool		m_bSpotLightActiveButton;
	bool		m_bPointLightActiveButton[4];

	uint8_t		m_totalPointLights;
};

