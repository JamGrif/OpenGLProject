#include "UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "EngineStatics.h"
#include "Input.h"
#include "OpenGLWindow.h"

UI::UI(bool uiVisible)
	:m_uiVisible(uiVisible), m_sceneNum(0),
	m_directionalLightInScene(false), m_directionalLightActive(true), 
	m_spotLightInScene(false), m_spotLightActive(true),
	m_pointLightInScene{ false, false, false, false }, m_pointLightActive{true, true, true, true},
	m_localLightManager(nullptr)
{
	std::cout << "UI Initialized" << std::endl;

	if (m_uiVisible)
	{
		Input::enableMouse();
	}
	else
	{
		Input::disableMouse();
	}

	IMGUI_CHECKVERSION(); // Check the version
	
	ImGui::CreateContext();	// Creating the imgui context
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	
	ImGui_ImplGlfw_InitForOpenGL(EngineStatics::getAppWindow()->getWindow(), true); // Connect imgui to glfw window
	ImGui_ImplOpenGL3_Init("#version 430");
	ImGui::StyleColorsClassic(); 
}

UI::~UI()
{
	std::cout << "UI destroyed" << std::endl;
}

void UI::startOfFrame()
{
	if (!m_uiVisible)
		return;
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UI::drawInFrame()
{
	if (!m_uiVisible)
		return;

	if (m_localLightManager == nullptr)
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
		ImGui::Checkbox("DirectionalLight", &m_directionalLightActive);

		// Set the active state of the DirectionalLight depending on the check box status
		m_localLightManager->getDirectionalLight(0)->lightActive = m_directionalLightActive ? true : false;
	}
	
	if (m_spotLightInScene)
	{
		// There is a spotLight in scene so show button and act on button presses
		ImGui::Checkbox("SpotLight", &m_spotLightActive);

		// Set the active state of the SpotLight depending on the check box status
		EngineStatics::getLightManager()->getSpotLight(0)->lightActive = m_spotLightActive ? true : false;
	}

	if (m_pointLightInScene[e_FirstPointLight])
	{
		// There is the first pointLight in scene and act on button presses
		ImGui::Checkbox("First PointLight", &m_pointLightActive[e_FirstPointLight]);

		// Set the active state of the pointLight depending on the check box status
		EngineStatics::getLightManager()->getPointLight(e_FirstPointLight)->lightActive = m_pointLightActive[e_FirstPointLight] ? true : false;
	}

	if (m_pointLightInScene[e_SecondPointLight])
	{
		// There is the second pointLight in scene and act on button presses
		ImGui::Checkbox("Second PointLight", &m_pointLightActive[e_SecondPointLight]);

		// Set the active state of the pointLight depending on the check box status
		EngineStatics::getLightManager()->getPointLight(e_SecondPointLight)->lightActive = m_pointLightActive[e_SecondPointLight] ? true : false;
	}

	if (m_pointLightInScene[e_ThirdPointLight])
	{
		// There is the third pointLight in scene and act on button presses
		ImGui::Checkbox("Third PointLight", &m_pointLightActive[e_ThirdPointLight]);

		// Set the active state of the pointLight depending on the check box status
		EngineStatics::getLightManager()->getPointLight(e_ThirdPointLight)->lightActive = m_pointLightActive[e_ThirdPointLight] ? true : false;
	}

	if (m_pointLightInScene[e_FourthPointLight])
	{
		// There is the fourth pointLight in scene and act on button presses
		ImGui::Checkbox("Fourth PointLight", &m_pointLightActive[e_FourthPointLight]);

		// Set the active state of the pointLight depending on the check box status
		EngineStatics::getLightManager()->getPointLight(e_FourthPointLight)->lightActive = m_pointLightActive[e_FourthPointLight] ? true : false;
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
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
}

/// <summary>
/// Toggles whether if the UI is visible or not
/// </summary>
void UI::toggleUI()
{
	m_uiVisible = m_uiVisible == true ? false : true;

	if (m_uiVisible)
	{
		Input::enableMouse();
	}
	else
	{
		Input::disableMouse();
	}
}

bool UI::getUiVisible()
{
	return m_uiVisible;
}

int UI::getSceneNum()
{
	return m_sceneNum;
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
