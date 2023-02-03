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

	bool		GetVisible() const { return m_bVisible; }
	SceneName	GetCurrentSceneName() const;

	void		UpdateSceneHandle(std::weak_ptr<Scene> newLoadedScene);

private:

	bool		m_bVisible;

	std::vector<IPanel*> m_allPanels;
};

