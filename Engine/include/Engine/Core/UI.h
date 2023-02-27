#pragma once

class IPanel;

/// <summary>
/// A wrapper around ImGui to display various UI panels to the screen
/// </summary>
class UI
{
public:
	UI(bool bVisible, Scene** pScenePointer);
	~UI();

	void		RenderUI();

	void		ToggleUI();

	void		ChangePanelScene();

private:

	// Each panel the UI will update and render every frame
	std::vector<std::shared_ptr<IPanel>> m_allPanels;

	// Whether the panels of the UI will be updated and rendered
	bool		m_bVisible;
};
