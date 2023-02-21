#include "pch.h"
#include "UI/ControlPanel.h"

#include "imgui.h"

ControlPanel::ControlPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer)
	:IPanel(panelName, imGuiWindowFlag, bVisible, pScenePointer)
{
}

ControlPanel::~ControlPanel()
{
}

/// <summary>
/// Inherited from IPanel
/// Unused in this class
/// </summary>
void ControlPanel::Update()
{
}

/// <summary>
/// Inherited from IPanel
/// Displays the controls of the scene camera
/// </summary>
void ControlPanel::Render()
{
	ImGui::Text("W/A/S/D to move around the camera");
	ImGui::Text("Moving the mouse moves the front facing vector of the camera");
	ImGui::Text("Q to toggle the UI");
}

/// <summary>
/// Inherited from IPanel
/// Unused in this class
/// </summary>
void ControlPanel::SceneChange()
{
}
