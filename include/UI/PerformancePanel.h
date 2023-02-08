#pragma once
#include "UI/IPanel.h"

/// <summary>
/// Displays the current FPS and delta time of the application
/// </summary>
class PerformancePanel :
	public IPanel
{
public:
	PerformancePanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer);
	~PerformancePanel();

	virtual void Update() override;
	virtual void Render() override;
	virtual void SceneChange() override;

private:
};
