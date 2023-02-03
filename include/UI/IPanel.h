#pragma once

enum class RenderFlag
{
	COMMON_RESIZE_FLAGS = 0,
	COMMON_FLAGS = 1,
	DEBUG_FLAGS = 2
};

class IPanel
{
public:
	IPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag);
	virtual ~IPanel();

	void StartOfFrame();
	void EndOfFrame();

	const std::string& getPanelName() const { return m_panelName; }
	ImGuiWindowFlags getFlag() const { return m_imGuiWindowFlag; }

	// What the panel does every frame
	virtual void Render() = 0;

	// What the panel does when the scene changes
	virtual void SceneChange() = 0;

	virtual void updateSceneHandle(std::weak_ptr<Scene> sceneHandle) { m_sceneHandle = sceneHandle; }

protected:

	std::string m_panelName;

	ImGuiWindowFlags m_imGuiWindowFlag;

	std::weak_ptr<Scene>	m_sceneHandle;
};

