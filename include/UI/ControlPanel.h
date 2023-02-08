#pragma once
#include "UI/IPanel.h"

/// <summary>
/// Displays the keyboard and mouse controls to move the SceneCamera
/// </summary>
class ControlPanel :
	public IPanel
{
public:
	ControlPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer);
	~ControlPanel();

	virtual void Update() override;
	virtual void Render() override; 
	virtual void SceneChange() override;

private:
};
