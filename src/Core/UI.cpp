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

#include <iostream>

// Flags for each ImGui window used
static constexpr ImGuiWindowFlags commonResizeFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
static constexpr ImGuiWindowFlags commonFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
static constexpr ImGuiWindowFlags debugFlags = ImGuiWindowFlags_NoCollapse;

UI::UI(bool uiVisible)
	:m_uiVisible(uiVisible),
	m_bDirectionalLightInScene(false), m_bDirectionalLightActiveButton(true),
	m_bSpotLightInScene(false), m_bSpotLightActiveButton(true),
	m_totalPointLights(0), m_bPointLightInScene{ false, false, false, false }, m_bPointLightActiveButton{ true, true, true, true },
	m_appPostProcess(0), m_currentSceneName(SceneName::UNSET_SCENE)
{
	PRINT_TRACE("UI Initialized");

	// Set initial mouse visibility
	m_uiVisible ? InputHandler::Instance()->EnableMouse() : InputHandler::Instance()->DisableMouse();

	// Create the imgui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.0f;
	style.FrameRounding = 4.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;

	// Connect ImGui to GLFW window
	ImGui_ImplGlfw_InitForOpenGL(TheOpenGLWindow::Instance()->GetWindowPtr(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	// Set ImGui colour style
	ImGui::StyleColorsClassic();
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Draw the various UI panels
	SceneOptionsPanel();
	ControlsPanel();
	PerformanceMetricsPanel();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// <summary>
/// Toggles whether if the UI is visible or not
/// </summary>
void UI::ToggleUI()
{
	// Flip variable state
	m_uiVisible = !m_uiVisible;

	// Toggle mouse depending on UI visibility
	m_uiVisible ? InputHandler::Instance()->EnableMouse() : InputHandler::Instance()->DisableMouse();
}

/// <summary>
/// Updates the cached pointer to the currently loaded scene
/// </summary>
void UI::UpdateSceneHandle(std::weak_ptr<Scene> newLoadedScene)
{
	m_sceneHandle = newLoadedScene;

	UpdateSceneInformation();
}

/// <summary>
/// Renders the Scene Options ImGui window
/// </summary>
void UI::SceneOptionsPanel()
{
	// Reset variable
	m_currentSceneName = SceneName::UNSET_SCENE;

	ImGui::Begin("Scene Options:", NULL, commonFlags);

	ImGui::Text("Change Scene:");
	if (ImGui::Button("FMPscene.txt"))
		m_currentSceneName = SceneName::FMPscene;
	
	if (ImGui::Button("jamieTest.txt"))
		m_currentSceneName = SceneName::jamieTest;
	
	if (ImGui::Button("lightTest.txt"))
		m_currentSceneName = SceneName::lightTest;
	
	if (ImGui::Button("materialTest.txt"))
		m_currentSceneName = SceneName::materialTest;
	
	if (ImGui::Button("shadowTest.txt"))
		m_currentSceneName = SceneName::shadowTest;
	
	ImGui::Text("Toggle Active Lights:");

	std::shared_ptr<SceneLightManager> sceneLM = m_sceneHandle.lock()->GetSceneLightManager();

	PRINT_TRACE("d{0}", sceneLM->GetCurrentDirectionalLights());
	PRINT_TRACE("p{0}", sceneLM->GetCurrentPointLights());
	PRINT_TRACE("s{0}", sceneLM->GetCurrentSpotLights());

	if (m_bDirectionalLightInScene)
	{
		PRINT_TRACE("Hi");
		// There is a directionalLight in scene so show button and act on button presses
		ImGui::Checkbox("DirectionalLight", &m_bDirectionalLightActiveButton);

		// Set the active state of the DirectionalLight depending on the check box status
		sceneLM->GetDirectionalLight(0).lock()->m_bLightActive = m_bDirectionalLightActiveButton ? true : false;

	}

	if (m_bSpotLightInScene)
	{
		// There is a spotLight in scene so show button and act on button presses
		ImGui::Checkbox("SpotLight", &m_bSpotLightActiveButton);

		// Set the active state of the SpotLight depending on the check box status
		sceneLM->GetSpotLight(0).lock()->m_bLightActive = m_bSpotLightActiveButton ? true : false;
	}

	for (int i = 0; i < m_totalPointLights; i++)
	{
		if (m_bPointLightInScene[i])
		{
			std::string nameTemp = "PointLight" + std::to_string(i);
			ImGui::Checkbox(nameTemp.c_str(), &m_bPointLightActiveButton[i]);

			// Set the active state of the pointLight depending on the check box status
			sceneLM->GetPointLight(i).lock()->m_bLightActive = m_bPointLightActiveButton[i] ? true : false;
		}
	}

	// Screen PostProcessing Filter
	m_appPostProcess = 0;

	ImGui::Text("Change Post-Processing Filter:");
	if (ImGui::Button("Normal"))
	{
		TheOpenGLRenderer::Instance()->SetScreenFilter(ScreenFilter::Default);
	}
	if (ImGui::Button("Inverse"))
	{
		TheOpenGLRenderer::Instance()->SetScreenFilter(ScreenFilter::Inverse);
	}
	if (ImGui::Button("Greyscale"))
	{
		TheOpenGLRenderer::Instance()->SetScreenFilter(ScreenFilter::Greyscale);
	}
	if (ImGui::Button("Edge Detection"))
	{
		TheOpenGLRenderer::Instance()->SetScreenFilter(ScreenFilter::EdgeDetection);
	}
	if (ImGui::Button("???"))
	{
		TheOpenGLRenderer::Instance()->SetScreenFilter(ScreenFilter::Weird);
	}

	ImGui::End();
}

/// <summary>
/// Renders the Controls ImGui window
/// </summary>
void UI::ControlsPanel()
{
	ImGui::Begin("Controls:", NULL, commonResizeFlags);

	ImGui::Text("W/A/S/D to move around the camera");
	ImGui::Text("Moving the mouse moves the front facing vector of the camera");
	ImGui::Text("Q to toggle the UI");

	ImGui::End();
}

/// <summary>
/// Renders the Performance Metrics ImGui window
/// </summary>
void UI::PerformanceMetricsPanel()
{
	std::string fps = "FPS: " + std::to_string(ApplicationClock::GetFrameCount());
	std::string delta = "Delta Time: " + std::to_string(ApplicationClock::GetDeltaTime());

	ImGui::Begin("Performance Metrics:", NULL, commonResizeFlags);

	ImGui::Text(fps.c_str());
	ImGui::Text(delta.c_str());

	ImGui::End();
}

/// <summary>
/// Function is called every time the current scene changes. It updates the available buttons light buttons depending on what lights the scene has
/// </summary>
void UI::UpdateSceneInformation()
{
	// Get the new scene light manager
	std::shared_ptr<SceneLightManager> sceneLM = m_sceneHandle.lock()->GetSceneLightManager();

	// Test whether there is a directionalLight in the scene
	m_bDirectionalLightInScene = sceneLM->GetCurrentDirectionalLights() > 0 ? true : false;

	// Test whether there is a spotLight in the scene
	m_bSpotLightInScene = sceneLM->GetCurrentSpotLights() > 0 ? true : false;

	enum
	{
		e_FirstPointLight  = 0,
		e_SecondPointLight = 1,
		e_ThirdPointLight  = 2,
		e_FourthPointLight = 3
	};

	// Test how many point lights are in the scene
	m_totalPointLights = sceneLM->GetCurrentPointLights();
	m_bPointLightInScene[e_FirstPointLight] = m_totalPointLights >= 1 ? true : false;
	m_bPointLightInScene[e_SecondPointLight] = m_totalPointLights >= 2 ? true : false;
	m_bPointLightInScene[e_ThirdPointLight] = m_totalPointLights >= 3 ? true : false;
	m_bPointLightInScene[e_FourthPointLight] = m_totalPointLights >= 4 ? true : false;
}
