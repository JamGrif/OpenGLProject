#include "pch.h"
#include "UI/OptionsPanel.h"

#include "imgui.h"

#include "Rendering/OpenGLRenderer.h"
#include "Scene/SceneLightManager.h"
#include "Scene/Scene.h"

OptionsPanel::OptionsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag)
	:IPanel(panelName, imGuiWindowFlag),
	m_bDirectionalLightInScene(false), m_bDirectionalLightActiveButton(true),
	m_bSpotLightInScene(false), m_bSpotLightActiveButton(true),
	m_totalPointLights(0), m_bPointLightInScene{ false, false, false, false }, m_bPointLightActiveButton{ true, true, true, true },
	m_currentSceneName(SceneName::UNSET_SCENE)
{
}

OptionsPanel::~OptionsPanel()
{
}

void OptionsPanel::Render()
{
	StartOfFrame();

	// Reset variable
	m_currentSceneName = SceneName::UNSET_SCENE;
	
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

	EndOfFrame();
}

void OptionsPanel::SceneChange()
{
	// Get new scene lightmanager
	std::shared_ptr<SceneLightManager> sceneLM = m_sceneHandle.lock()->GetSceneLightManager();

	// Check if directionalLight in scene
	m_bDirectionalLightInScene = sceneLM->GetCurrentDirectionalLights() > 0 ? true : false;

	// Check if spotLight in scene
	m_bSpotLightInScene = sceneLM->GetCurrentSpotLights() > 0 ? true : false;

	// Reset status of each array element only if some are set
	if (m_totalPointLights > 0)
	{
		for (int i = 0; i < m_totalPointLights; i++)
		{
			m_bPointLightInScene[i] = false;
		}
	}

	// Check how many point lights in scene
	m_totalPointLights = sceneLM->GetCurrentPointLights();

	// Set status for number of point lights
	for (int i = 0; i < m_totalPointLights; i++)
	{
		m_bPointLightInScene[i] = true;
	}
}
