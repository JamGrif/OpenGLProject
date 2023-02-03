#include "pch.h"
#include "UI/PerformancePanel.h"

#include "imgui.h"

PerformancePanel::PerformancePanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag)
	:IPanel(panelName, imGuiWindowFlag)
{
}

PerformancePanel::~PerformancePanel()
{
}

void PerformancePanel::Render()
{
	StartOfFrame();

	ImGui::Text("FPS:");
	ImGui::SameLine();
	ImGui::Text(std::to_string(ApplicationClock::Get()->GetFrameCount()).c_str());

	ImGui::Text("Delta Time:");
	ImGui::SameLine();
	ImGui::Text(std::to_string(ApplicationClock::Get()->GetDeltaTime()).c_str());

	EndOfFrame();
}

void PerformancePanel::SceneChange()
{
}
