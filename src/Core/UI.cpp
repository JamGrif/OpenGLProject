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

// Flags for each ImGui window used
static ImGuiWindowFlags commonResizeFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
static ImGuiWindowFlags commonFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
static ImGuiWindowFlags debugFlags = ImGuiWindowFlags_NoCollapse;

UI::UI(bool uiVisible, std::shared_ptr<Scene> loadedScene)
	:m_uiVisible(uiVisible), m_sceneHandle(loadedScene), m_sceneNum(0),
	m_directionalLightInScene(false), m_directionalLightActiveButton(true),
	m_spotLightInScene(false), m_spotLightActiveButton(true),
	m_totalPointLights(0), m_pointLightInScene{ false, false, false, false }, m_pointLightActiveButton{ true, true, true, true },
	m_appPostProcess(0)
{
	PRINT_TRACE("UI Initialized");

	// Enable or Disable the mouse depending on UI visibility
	m_uiVisible ? InputHandler::Instance()->EnableMouse() : InputHandler::Instance()->DisableMouse();

	// Check the version
	IMGUI_CHECKVERSION();

	// Create the imgui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowRounding = 5.0f;
	style->FrameRounding = 4.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;

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
	//sceneEntitiesPanel();

	//if (m_isEntitySelected)
	//	entityPanel();
	
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
/// <param name="newLoadedScene">Pointer to the new scene</param>
void UI::UpdateSceneHandle(std::shared_ptr<Scene> newLoadedScene)
{
	m_sceneHandle = newLoadedScene;

	// If newLoadedScene was set to be null then don't update any information
	if (!newLoadedScene)
		return;

	UpdateSceneInformation();
}

/// <summary>
/// Renders the Scene Options ImGui window
/// </summary>
void UI::SceneOptionsPanel()
{
	// Reset variable
	m_sceneNum = 0;

	ImGui::Begin("Scene Options:", NULL, commonFlags);

	ImGui::Text("Change Scene:");
	if (ImGui::Button("FMPscene.txt"))
	{
		m_sceneNum = e_FMPscene;
	}
	if (ImGui::Button("jamieTest.txt"))
	{
		m_sceneNum = e_jamieTest;
	}
	if (ImGui::Button("lightTest.txt"))
	{
		m_sceneNum = e_lightTest;
	}
	if (ImGui::Button("materialTest.txt"))
	{
		m_sceneNum = e_materialTest;
	}
	if (ImGui::Button("shadowTest.txt"))
	{
		m_sceneNum = e_shadowTest;
	}

	ImGui::Text("Toggle Active Lights:");

	std::shared_ptr<SceneLightManager> sceneLM = m_sceneHandle->GetSceneLightManager();

	if (m_directionalLightInScene)
	{
		// There is a directionalLight in scene so show button and act on button presses
		ImGui::Checkbox("DirectionalLight", &m_directionalLightActiveButton);

		// Set the active state of the DirectionalLight depending on the check box status
		sceneLM->GetDirectionalLight(0).lock()->m_bLightActive = m_directionalLightActiveButton ? true : false;

	}

	if (m_spotLightInScene)
	{
		// There is a spotLight in scene so show button and act on button presses
		ImGui::Checkbox("SpotLight", &m_spotLightActiveButton);

		// Set the active state of the SpotLight depending on the check box status
		sceneLM->GetSpotLight(0).lock()->m_bLightActive = m_spotLightActiveButton ? true : false;
	}

	for (int i = 0; i < m_totalPointLights; i++)
	{
		if (m_pointLightInScene[i])
		{
			std::string nameTemp = "PointLight" + std::to_string(i);
			ImGui::Checkbox(nameTemp.c_str(), &m_pointLightActiveButton[i]);

			// Set the active state of the pointLight depending on the check box status
			sceneLM->GetPointLight(i).lock()->m_bLightActive = m_pointLightActiveButton[i] ? true : false;
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
	std::shared_ptr<SceneLightManager> sceneLM = m_sceneHandle->GetSceneLightManager();

	// Test whether there is a directionalLight in the scene
	m_directionalLightInScene = sceneLM->GetCurrentDirectionalLights() > 0 ? true : false;

	// Test whether there is a spotLight in the scene
	m_spotLightInScene = sceneLM->GetCurrentSpotLights() > 0 ? true : false;

	enum
	{
		e_FirstPointLight  = 0,
		e_SecondPointLight = 1,
		e_ThirdPointLight  = 2,
		e_FourthPointLight = 3
	};

	// Test how many point lights are in the scene
	m_totalPointLights = sceneLM->GetCurrentPointLights();
	m_pointLightInScene[e_FirstPointLight] = m_totalPointLights >= 1 ? true : false;
	m_pointLightInScene[e_SecondPointLight] = m_totalPointLights >= 2 ? true : false;
	m_pointLightInScene[e_ThirdPointLight] = m_totalPointLights >= 3 ? true : false;
	m_pointLightInScene[e_FourthPointLight] = m_totalPointLights >= 4 ? true : false;
}
