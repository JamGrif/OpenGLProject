#include "pch.h"
#include "UI/ControlPanel.h"

#include "imgui.h"

ControlPanel::ControlPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag)
	:IPanel(panelName, imGuiWindowFlag)
{
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::Render()
{
	StartOfFrame();

	ImGui::Text("W/A/S/D to move around the camera");
	ImGui::Text("Moving the mouse moves the front facing vector of the camera");
	ImGui::Text("Q to toggle the UI");

	EndOfFrame();
}

void ControlPanel::SceneChange()
{
}
