#include "pch.h"
#include "Core/UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Rendering/OpenGLWindow.h"
#include "Scene/Scene.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Model.h"
#include "Core/InputHandler.h"
#include "Rendering/OpenGLRenderer.h"

#include "UI/OptionsPanel.h"
#include "UI/ControlPanel.h"
#include "UI/PerformancePanel.h"
#include "UI/SceneModelsPanel.h"
#include "UI/ModelDataPanel.h"

// Flags for each ImGui window used
static constexpr ImGuiWindowFlags COMMON_RESIZE_FLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
static constexpr ImGuiWindowFlags COMMON_FLAGS = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
static constexpr ImGuiWindowFlags DEBUG_FLAGS = ImGuiWindowFlags_NoCollapse;

UI::UI(bool bVisible)
	:m_bVisible(bVisible)
{
	PRINT_TRACE("UI Initialized");

	// Set initial mouse visibility
	m_bVisible ? InputHandler::Get()->EnableMouse() : InputHandler::Get()->DisableMouse();

	// Create the imgui context
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.0f;
	style.FrameRounding = 4.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;

	// Connect ImGui to GLFW window
	ImGui_ImplGlfw_InitForOpenGL(TheOpenGLWindow::Get()->GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	// Set ImGui colour style
	ImGui::StyleColorsClassic();

	m_allPanels.emplace_back(new ControlPanel("Controls", COMMON_FLAGS));
	m_allPanels.emplace_back(new PerformancePanel("Performance Metrics", COMMON_FLAGS));
	m_allPanels.emplace_back(new OptionsPanel("Scene Options", COMMON_FLAGS));
	m_allPanels.emplace_back(new SceneModelsPanel("Scene Models", COMMON_FLAGS));
	m_allPanels.emplace_back(new ModelDataPanel("Model Data", COMMON_FLAGS, dynamic_cast<SceneModelsPanel*>(m_allPanels.back())));
}

UI::~UI()
{
	PRINT_TRACE("UI Destroyed");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/// <summary>
/// Called every frame to draw the UI and adjust variables depending on user input on the ImGui buttons
/// </summary>
void UI::RenderUI()
{
	// Setup new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Render all panels
	for (const auto& panel : m_allPanels)
	{
		//ImGui::Begin(panel->getPanelName().c_str(), NULL, panel->getFlag());

		panel->Render();

		//ImGui::End();
	}

	// Finalize frame
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// <summary>
/// Toggles whether if the UI is visible or not
/// </summary>
void UI::ToggleUI()
{
	// Flip variable state
	m_bVisible = !m_bVisible;

	// Toggle mouse depending on UI visibility
	m_bVisible ? InputHandler::Get()->EnableMouse() : InputHandler::Get()->DisableMouse();
}

SceneName UI::GetCurrentSceneName() const
{
	for (const auto& panel : m_allPanels)
	{
		if (dynamic_cast<OptionsPanel*>(panel))
		{
			return dynamic_cast<OptionsPanel*>(panel)->GetCurrentSceneName();
		}
	}

	return SceneName::UNSET_SCENE;
}

/// <summary>
/// Updates the cached pointer to the currently loaded scene
/// </summary>
void UI::UpdateSceneHandle(std::weak_ptr<Scene> newLoadedScene)
{
	for (const auto& panel : m_allPanels)
	{
		panel->updateSceneHandle(newLoadedScene);
		panel->SceneChange();
	}
}
