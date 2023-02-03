#pragma once
#include "UI/IPanel.h"

enum class SceneName;

class OptionsPanel :
    public IPanel
{
public:
	OptionsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag);
	~OptionsPanel();

	void Render() override;

	void SceneChange() override;

	SceneName GetCurrentSceneName() { return m_currentSceneName; }

private:

	SceneName	m_currentSceneName;

	bool		m_bDirectionalLightInScene; // Indicates if the button to toggle the light should be visible or not
	bool		m_bDirectionalLightActiveButton;

	bool		m_bSpotLightInScene;
	bool		m_bSpotLightActiveButton;

	uint8_t		m_totalPointLights;
	bool		m_bPointLightInScene[4];
	bool		m_bPointLightActiveButton[4];
};

