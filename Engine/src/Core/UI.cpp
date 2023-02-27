#include "pch.h"
#include "Core/UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Rendering/OpenGLWindow.h"
#include "Scene/Scene.h"
#include "Core/InputHandler.h"

#include "UI/OptionsPanel.h"
#include "UI/ControlPanel.h"
#include "UI/PerformancePanel.h"
#include "UI/SceneModelsPanel.h"
#include "UI/ModelDataPanel.h"

// Flags for each ImGui window used
static constexpr ImGuiWindowFlags COMMON_RESIZE_FLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
static constexpr ImGuiWindowFlags COMMON_FLAGS = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
static constexpr ImGuiWindowFlags DEBUG_FLAGS = ImGuiWindowFlags_NoCollapse;

UI::UI(bool bVisible, Scene** pScenePointer)
	:m_bVisible(bVisible)
{
	// Set initial mouse visibility
	m_bVisible ? InputHandler::Get()->EnableMouse() : InputHandler::Get()->DisableMouse();

	// Create the imgui context
	ImGui::CreateContext();
	ImGui::GetIO().IniFilename = "config/imguiconfig.ini";

	// Specify style of ImGui
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding	= 5.0f;
	style.FrameRounding		= 4.0f;
	style.ScrollbarSize		= 15.0f;
	style.ScrollbarRounding = 9.0f;

	// Connect ImGui to GLFW window
	ImGui_ImplGlfw_InitForOpenGL(TheOpenGLWindow::Get()->GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	// Set ImGui colour style
	ImGui::StyleColorsClassic();

	// Create each panel
	m_allPanels.emplace_back(std::make_shared<ControlPanel>("Controls", COMMON_FLAGS, true, pScenePointer));
	m_allPanels.emplace_back(std::make_shared<PerformancePanel>("Performance Metrics", COMMON_FLAGS, true, pScenePointer));
	m_allPanels.emplace_back(std::make_shared<OptionsPanel>("Scene Options", COMMON_FLAGS, true, pScenePointer));
	m_allPanels.emplace_back(std::make_shared<SceneModelsPanel>("Scene Models", COMMON_FLAGS, true, pScenePointer));
	m_allPanels.emplace_back(std::make_shared<ModelDataPanel>("Model Data", COMMON_FLAGS, false, pScenePointer, std::dynamic_pointer_cast<SceneModelsPanel>(m_allPanels.back())));
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/// <summary>
/// Render each panel of the UI
/// </summary>
void UI::RenderUI()
{
	if (!m_bVisible)
		return;

	// Setup new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Render all panels
	for (const auto& panel : m_allPanels)
	{
		panel->Update();

		if (panel->GetVisible())
		{
			ImGui::Begin(panel->GetPanelName().c_str(), NULL, panel->GetFlag());

			panel->Render();

			ImGui::End();
		}
	}

	// Finalize frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// <summary>
/// Toggle visibility of UI
/// </summary>
void UI::ToggleUI()
{
	// Flip state
	m_bVisible = !m_bVisible;

	// Toggle mouse
	m_bVisible ? InputHandler::Get()->EnableMouse() : InputHandler::Get()->DisableMouse();
}

/// <summary>
/// Update the scene handle of each UI panel
/// </summary>
void UI::ChangePanelScene()
{
	for (const auto& panel : m_allPanels)
	{
		panel->SceneChange();
	}
}
