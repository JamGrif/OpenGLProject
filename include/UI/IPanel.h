#pragma once

// Flag for rendering panel
enum class RenderFlag
{
	COMMON_RESIZE_FLAGS = 0,
	COMMON_FLAGS		= 1,
	DEBUG_FLAGS			= 2
};

/// <summary>
/// Base class for a UI panel
/// </summary>
class IPanel
{
public:
	IPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer);
	virtual ~IPanel();

	// What the panel updates every frame
	virtual void			Update() = 0;

	// What the panel renders every frame
	virtual void			Render() = 0;

	// What the panel does when scene changes
	virtual void			SceneChange() = 0;

	const std::string&		GetPanelName() const { return m_panelName; }
	ImGuiWindowFlags		GetFlag() const { return m_imGuiWindowFlag; }
	bool					GetVisible() const { return m_bVisible; }

	//virtual void			UpdateSceneHandle(std::weak_ptr<Scene> pSceneHandle) { m_pSceneHandle = pSceneHandle; }

protected:

	// Label of panel when rendered
	std::string				m_panelName;

	// Rendering flag for panel
	ImGuiWindowFlags		m_imGuiWindowFlag;

	// The currently loaded scene
	Scene** m_pSceneHandle;

	// Whether the panel will be rendered
	bool					m_bVisible;
};
