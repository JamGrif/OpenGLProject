#include "UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "EngineStatics.h"
#include "Input.h"
#include "OpenGLWindow.h"
#include "GameTimer.h"
#include "LightManager.h"

UI::UI(bool uiVisible)
	:m_uiVisible(uiVisible), m_sceneNum(0),
	m_directionalLightInScene(false), m_directionalLightActiveButton(true),
	m_spotLightInScene(false), m_spotLightActiveButton(true),
	m_pointLightInScene{ false, false, false, false }, m_pointLightActiveButton{ true, true, true, true },
	m_appPostProcess(0), m_localLightManager(nullptr), m_localgameTimer(EngineStatics::getGameTimer())
{
	std::cout << "UI Initialized" << std::endl;

	// Enable or Disable the mouse depending on UI visibility
	m_uiVisible ? Input::enableMouse() : Input::disableMouse();

	// Check the version
	IMGUI_CHECKVERSION(); 

	// Create the imgui context
	ImGui::CreateContext();	
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Connect ImGui to GLFW window
	ImGui_ImplGlfw_InitForOpenGL(EngineStatics::getAppWindow()->getWindow(), true); 
	ImGui_ImplOpenGL3_Init("#version 430");

	// Set ImGui colour style
	ImGui::StyleColorsClassic(); 
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	std::cout << "UI destroyed" << std::endl;
}

/// <summary>
/// Called at start of each frame, before UI drawInFrame()
/// </summary>
void UI::startOfFrame()
{
	// Check if user wants to toggle UI visibility
	if (Input::getKeyPressedOnce(GLFW_KEY_Q))
	{
		toggleUI();
	}

	// If UI is not visible then return
	if (!m_uiVisible)
		return;
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

/// <summary>
/// Called every frame to draw the UI and adjust variables depending on user input on the ImGui buttons
/// </summary>
void UI::drawInFrame()
{
	// UI is not visible so leave
	if (!m_uiVisible)
		return;

	// If localLightManager is nullptr then the scene hasn't been loaded yet
	if (!m_localLightManager)
		return;

	/*
		Scene Options TextBox
	*/

	// Reset variable
	m_sceneNum = 0;

	ImGui::Begin("Scene Options");
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

	if (m_directionalLightInScene) 
	{
		// There is a directionalLight in scene so show button and act on button presses
		ImGui::Checkbox("DirectionalLight", &m_directionalLightActiveButton);

		// Set the active state of the DirectionalLight depending on the check box status
		m_localLightManager->getDirectionalLight(0)->lightActive = m_directionalLightActiveButton ? true : false;
	}
	
	if (m_spotLightInScene)
	{
		// There is a spotLight in scene so show button and act on button presses
		ImGui::Checkbox("SpotLight", &m_spotLightActiveButton);

		// Set the active state of the SpotLight depending on the check box status
		m_localLightManager->getSpotLight(0)->lightActive = m_spotLightActiveButton ? true : false;
	}


	if (m_pointLightInScene[e_FirstPointLight])
	{
		// There is the first pointLight in scene and act on button presses
		ImGui::Checkbox("First PointLight", &m_pointLightActiveButton[e_FirstPointLight]);
	
		// Set the active state of the pointLight depending on the check box status
		m_localLightManager->getPointLight(e_FirstPointLight)->lightActive = m_pointLightActiveButton[e_FirstPointLight] ? true : false;
	}
	
	if (m_pointLightInScene[e_SecondPointLight])
	{
		// There is the second pointLight in scene and act on button presses
		ImGui::Checkbox("Second PointLight", &m_pointLightActiveButton[e_SecondPointLight]);
	
		// Set the active state of the pointLight depending on the check box status
		m_localLightManager->getPointLight(e_SecondPointLight)->lightActive = m_pointLightActiveButton[e_SecondPointLight] ? true : false;
	}
	
	if (m_pointLightInScene[e_ThirdPointLight])
	{
		// There is the third pointLight in scene and act on button presses
		ImGui::Checkbox("Third PointLight", &m_pointLightActiveButton[e_ThirdPointLight]);
	
		// Set the active state of the pointLight depending on the check box status
		m_localLightManager->getPointLight(e_ThirdPointLight)->lightActive = m_pointLightActiveButton[e_ThirdPointLight] ? true : false;
	}
	
	if (m_pointLightInScene[e_FourthPointLight])
	{
		// There is the fourth pointLight in scene and act on button presses
		ImGui::Checkbox("Fourth PointLight", &m_pointLightActiveButton[e_FourthPointLight]);
	
		// Set the active state of the pointLight depending on the check box status
		m_localLightManager->getPointLight(e_FourthPointLight)->lightActive = m_pointLightActiveButton[e_FourthPointLight] ? true : false;
	}

	// Screen PostProcessing Filter
	m_appPostProcess = 0;

	ImGui::Text("Change Post-Processing Filter:");
	if (ImGui::Button("Normal"))
	{
		m_appPostProcess = 1;
	}
	if (ImGui::Button("Inverse"))
	{
		m_appPostProcess = 2;
	}
	if (ImGui::Button("Greyscale"))
	{
		m_appPostProcess = 3;
	}
	if (ImGui::Button("Edge Detection"))
	{
		m_appPostProcess = 4;
	}
	if (ImGui::Button("???"))
	{
		m_appPostProcess = 5;
	}

	ImGui::End();

	/*
		Controls TextBox
	*/

	ImGui::Begin("Controls:");
	ImGui::Text("W/A/S/D to move around the camera");
	ImGui::Text("Moving the mouse moves the front facing vector of the camera");
	ImGui::Text("Q to toggle the UI");
	ImGui::Text("1/2/3/4/5 to change the screen filter applied to the drawn frame");
	ImGui::End();

	/*
		Performance Metrics
	*/

	double x = m_localgameTimer->getDeltaTime();
	std::string xx = std::to_string(x);
	std::string xxx = "Delta Time: " + xx;

	double y = m_localgameTimer->getFrameCount();
	std::string yy = std::to_string(y);
	std::string yyy = "FPS: " + yy;
	

	ImGui::Begin("Performance Metrics:");
	ImGui::Text(yyy.c_str());
	ImGui::Text(xxx.c_str());
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// <summary>
/// Toggles whether if the UI is visible or not
/// </summary>
void UI::toggleUI()
{
	// Flip variable state
	m_uiVisible = m_uiVisible == true ? false : true;

	// Toggle mouse depending on UI visibility
	m_uiVisible ? Input::enableMouse() : Input::disableMouse();
}

bool UI::getUiVisible() const
{
	return m_uiVisible;
}

int UI::getSceneNum() const
{
	return m_sceneNum;
}

int UI::getFilterNum() const
{
	return m_appPostProcess;
}

/// <summary>
/// Function is called every time the current scene changes. It updates the available buttons light buttons depending on what lights the scene has
/// </summary>
void UI::refreshLightButtons()
{
	m_localLightManager = EngineStatics::getLightManager();

	// Test whether there is a directionalLight in the scene
	m_directionalLightInScene = m_localLightManager->getCurrentDirectionalLights() > 0 ? true : false;

	// Test whether there is a spotLight in the scene
	m_spotLightInScene = m_localLightManager->getCurrentSpotLights() > 0 ? true : false;

	// Test how many point lights are in the scene
	int pointLightNum = m_localLightManager->getCurrentPointLights();
	m_pointLightInScene[e_FirstPointLight]	= pointLightNum >= 1 ? true : false;
	m_pointLightInScene[e_SecondPointLight] = pointLightNum >= 2 ? true : false;
	m_pointLightInScene[e_ThirdPointLight]	= pointLightNum >= 3 ? true : false;
	m_pointLightInScene[e_FourthPointLight] = pointLightNum >= 4 ? true : false;
}
