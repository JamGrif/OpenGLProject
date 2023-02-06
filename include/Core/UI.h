#pragma once

class IPanel;

enum class SceneName;

/// <summary>
/// A wrapper around ImGui to display UI elements to the screen
/// </summary>
class UI
{
public:
	UI(bool bVisible);
	~UI();

	void		RenderUI();

	void		ToggleUI();

	SceneName	GetCurrentSceneName() const;

	void		UpdateSceneHandle(std::weak_ptr<Scene> newLoadedScene);

private:

	// Each panel the UI will update and render every frame
	std::vector<std::shared_ptr<IPanel>> m_allPanels;

	// Whether the panels of the UI will be updated and rendered
	bool		m_bVisible;
};

