#include "pch.h"
#include "UI/PerformancePanel.h"

#include "imgui.h"

PerformancePanel::PerformancePanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible)
	:IPanel(panelName, imGuiWindowFlag, bVisible)
{
}

PerformancePanel::~PerformancePanel()
{
}

/// <summary>
/// Inherited from IPanel
/// Unused in this class
/// </summary>
void PerformancePanel::Update()
{
}

/// <summary>
/// Inherited from IPanel
/// Displays data from the application clock
/// </summary>
void PerformancePanel::Render()
{
	ImGui::Text("FPS: %i", ApplicationClock::Get()->GetFrameCount());

	ImGui::Text("Delta Time: %f", ApplicationClock::Get()->GetDeltaTime());
}

/// <summary>
/// Inherited from IPanel
/// Unused in this class
/// </summary>
void PerformancePanel::SceneChange()
{
}
