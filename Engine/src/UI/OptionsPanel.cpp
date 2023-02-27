#include "pch.h"
#include "UI/OptionsPanel.h"

#include "imgui.h"

#include "Rendering/OpenGLRenderer.h"
#include "Scene/SceneLightManager.h"
#include "Scene/Scene.h"

OptionsPanel::OptionsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer)
	:IPanel(panelName, imGuiWindowFlag, bVisible, pScenePointer),
	m_bDirectionalLightInScene(false), m_bDirectionalLightActiveButton(true),
	m_bSpotLightInScene(false), m_bSpotLightActiveButton(true),
	m_totalPointLights(0), m_bPointLightInScene{ false, false, false, false }, m_bPointLightActiveButton{ true, true, true, true }
{
}

OptionsPanel::~OptionsPanel()
{
}

/// <summary>
/// Inherited from IPanel
/// Unused in this class
/// </summary>
void OptionsPanel::Update()
{
}

/// <summary>
/// Inherited from IPanel
/// Displays all the buttons and checkboxes for the user to manipulate the scene
/// </summary>
void OptionsPanel::Render()
{
	ImGui::Text("Toggle Active Lights:");
	
	std::shared_ptr<SceneLightManager> sceneLM = (*m_pSceneHandle)->GetSceneLightManager().lock();
	
	if (m_bDirectionalLightInScene)
	{
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
	ImGui::Text("Change Post-Processing Filter:");
	if (ImGui::Button("Normal"))
		TheOpenGLRenderer::Get()->SetScreenFilter(ScreenFilter::Default);
	
	if (ImGui::Button("Inverse"))
		TheOpenGLRenderer::Get()->SetScreenFilter(ScreenFilter::Inverse);
	
	if (ImGui::Button("Greyscale"))
		TheOpenGLRenderer::Get()->SetScreenFilter(ScreenFilter::Greyscale);
	
	if (ImGui::Button("Edge Detection"))
		TheOpenGLRenderer::Get()->SetScreenFilter(ScreenFilter::EdgeDetection);
	
	if (ImGui::Button("???"))
		TheOpenGLRenderer::Get()->SetScreenFilter(ScreenFilter::Weird);
}

/// <summary>
/// Inherited from IPanel
/// Sets up the available light checkboxes depending on which ones are present in scene 
/// </summary>
void OptionsPanel::SceneChange()
{
	// Get new scene lightmanager
	std::shared_ptr<SceneLightManager> pLightManager = (*m_pSceneHandle)->GetSceneLightManager().lock();

	// Check how many point lights in scene
	m_totalPointLights = pLightManager->GetCurrentPointLights();

	// Check if directionalLight in scene
	m_bDirectionalLightInScene = pLightManager->GetCurrentDirectionalLights() > 0 ? true : false;

	// Check if spotLight in scene
	m_bSpotLightInScene = pLightManager->GetCurrentSpotLights() > 0 ? true : false;

	// Check if each pointLight in scene
	for (int i = 0; i < m_totalPointLights; i++)
	{
		m_bPointLightInScene[i] = true;
	}

	// Reset status of each light checkbox
	for (int i = 0; i < m_totalPointLights; i++)
	{
		m_bPointLightActiveButton[i] = true;
	}

	m_bDirectionalLightActiveButton = true;
	m_bSpotLightActiveButton = true;
}
