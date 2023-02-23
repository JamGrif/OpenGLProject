#include "pch.h"
#include "UI/PerformancePanel.h"

#include "imgui.h"

PerformancePanel::PerformancePanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer)
	:IPanel(panelName, imGuiWindowFlag, bVisible, pScenePointer)
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
	ImGui::Text("FPS: %i", EngineClock::Get()->GetFrameCount());

	ImGui::Text("Delta Time: %f", EngineClock::Get()->GetDeltaTime());
}

/// <summary>
/// Inherited from IPanel
/// Unused in this class
/// </summary>
void PerformancePanel::SceneChange()
{
}
