#include "pch.h"
#include "Core/UI.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Core/EngineStatics.h"
#include "Rendering/OpenGLWindow.h"
#include "Scene/Scene.h"
#include "Scene/SceneLightManager.h"
#include "Scene/EntityTypes/LightingEntity.h"

static ImGuiWindowFlags commonResizeFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
static ImGuiWindowFlags commonFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
static ImGuiWindowFlags debugFlags = ImGuiWindowFlags_NoCollapse;

UI::UI(bool uiVisible, std::shared_ptr<Scene> loadedScene)
	:m_isEntitySelected(false), m_uiVisible(uiVisible), m_sceneHandle(loadedScene), m_sceneNum(0),
	m_directionalLightInScene(false), m_directionalLightActiveButton(true),
	m_spotLightInScene(false), m_spotLightActiveButton(true),
	m_pointLightInScene{ false, false, false, false }, m_pointLightActiveButton{ true, true, true, true },
	m_appPostProcess(0)
{
	PRINT_TRACE("UI Initialized");

	// Enable or Disable the mouse depending on UI visibility
	m_uiVisible ? Input::enableMouse() : Input::disableMouse();

	// Check the version
	IMGUI_CHECKVERSION(); 

	// Create the imgui context
	ImGui::CreateContext();	
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Connect ImGui to GLFW window
	ImGui_ImplGlfw_InitForOpenGL(EngineStatics::getAppWindow()->getRaw(), true);
	ImGui_ImplOpenGL3_Init("#version 430");

	// Set ImGui colour style
	ImGui::StyleColorsClassic(); 
}

UI::~UI()
{
	PRINT_TRACE("UI Destroyed");

	//m_localLightManager = nullptr;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

/// <summary>
/// Called at start of each frame, before UI drawInFrame()
/// </summary>
void UI::startOfFrame()
{
	// Check if user wants to toggle UI visibility
	if (Input::getKeyPressedOnce(Keyboard::Q))
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
void UI::update()
{
	// UI is not visible so leave
	if (!m_uiVisible)
		return;

	// If localLightManager is nullptr then the scene hasn't been loaded yet
	//if (!m_localLightManager)
	//	return;

	// Draw the various UI panels
	sceneOptionsPanel();
	controlsPanel();
	performanceMetricsPanel();
	sceneEntitiesPanel();

	if (m_isEntitySelected)
		entityPanel();
	

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

void UI::updateSceneHandle(std::shared_ptr<Scene> newLoadedScene)
{
	m_sceneHandle = newLoadedScene;

	// If newLoadedScene was set to be null then don't update any information
	if (newLoadedScene == nullptr)
		return;

	updateSceneInformation();
}


void UI::sceneOptionsPanel()
{
	/*
		Scene Options TextBox
	*/

	// Reset variable
	m_sceneNum = 0;

	ImGui::Begin("Scene Options:", NULL, commonResizeFlags);
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

	std::shared_ptr<SceneLightManager> sceneLM = m_sceneHandle->getSceneLightManager();

	if (m_directionalLightInScene)
	{
		// There is a directionalLight in scene so show button and act on button presses
		ImGui::Checkbox("DirectionalLight", &m_directionalLightActiveButton);

		// Set the active state of the DirectionalLight depending on the check box status
		sceneLM->getDirectionalLight(0)->lightActive = m_directionalLightActiveButton ? true : false;
	}

	if (m_spotLightInScene)
	{
		// There is a spotLight in scene so show button and act on button presses
		ImGui::Checkbox("SpotLight", &m_spotLightActiveButton);

		// Set the active state of the SpotLight depending on the check box status
		sceneLM->getSpotLight(0)->lightActive = m_spotLightActiveButton ? true : false;
	}


	if (m_pointLightInScene[e_FirstPointLight])
	{
		// There is the first pointLight in scene and act on button presses
		ImGui::Checkbox("First PointLight", &m_pointLightActiveButton[e_FirstPointLight]);

		// Set the active state of the pointLight depending on the check box status
		sceneLM->getPointLight(e_FirstPointLight)->lightActive = m_pointLightActiveButton[e_FirstPointLight] ? true : false;
	}

	if (m_pointLightInScene[e_SecondPointLight])
	{
		// There is the second pointLight in scene and act on button presses
		ImGui::Checkbox("Second PointLight", &m_pointLightActiveButton[e_SecondPointLight]);

		// Set the active state of the pointLight depending on the check box status
		sceneLM->getPointLight(e_SecondPointLight)->lightActive = m_pointLightActiveButton[e_SecondPointLight] ? true : false;
	}

	if (m_pointLightInScene[e_ThirdPointLight])
	{
		// There is the third pointLight in scene and act on button presses
		ImGui::Checkbox("Third PointLight", &m_pointLightActiveButton[e_ThirdPointLight]);

		// Set the active state of the pointLight depending on the check box status
		sceneLM->getPointLight(e_ThirdPointLight)->lightActive = m_pointLightActiveButton[e_ThirdPointLight] ? true : false;
	}

	if (m_pointLightInScene[e_FourthPointLight])
	{
		// There is the fourth pointLight in scene and act on button presses
		ImGui::Checkbox("Fourth PointLight", &m_pointLightActiveButton[e_FourthPointLight]);

		// Set the active state of the pointLight depending on the check box status
		sceneLM->getPointLight(e_FourthPointLight)->lightActive = m_pointLightActiveButton[e_FourthPointLight] ? true : false;
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
}

void UI::controlsPanel()
{
	/*
		Controls TextBox
	*/

	ImGui::Begin("Controls:", NULL, commonResizeFlags);
	ImGui::Text("W/A/S/D to move around the camera");
	ImGui::Text("Moving the mouse moves the front facing vector of the camera");
	ImGui::Text("Q to toggle the UI");
	ImGui::Text("1/2/3/4/5 to change the screen filter applied to the drawn frame");
	ImGui::End();
}

void UI::performanceMetricsPanel()
{
	/*
		Performance Metrics
	*/

	std::string fps = "FPS: " + std::to_string(ApplicationClock::getFrameCount());
	std::string delta = "Delta Time: " + std::to_string(ApplicationClock::getDeltaTime());

	ImGui::Begin("Performance Metrics:", NULL, commonResizeFlags);
	ImGui::Text(fps.c_str());
	ImGui::Text(delta.c_str());
	ImGui::End();
}

void UI::sceneEntitiesPanel()
{
	ImGui::Begin("Scene Entities:", NULL, commonFlags);
	for (int i = 0; i < m_sceneHandle->getEntityNum(); i++)
	{
		std::string buttonName = m_sceneHandle->getEntityAtIndex(i)->getEntityType() + std::to_string(i);
		if (ImGui::Button(buttonName.c_str()))
		{
			if (i != m_selectedEntityIndex)
			{
				m_selectedEntityIndex = i;
				m_isEntitySelected = true;
			}
		}
	}
	
	ImGui::End();
}

constexpr int sameLineSpacing = 70;
void UI::entityPanel()
{
	ImGui::Begin("Entity", NULL, commonFlags);

	std::shared_ptr<LightingEntity> entity = m_sceneHandle->getEntityAtIndex(m_selectedEntityIndex);

	ImGui::Text("Entity Type:");
	ImGui::SameLine(95);
	ImGui::Text(entity->getEntityType().c_str());

	if (ImGui::Button("delete"))
	{
		m_sceneHandle->deleteLightingEntityFromVector(m_selectedEntityIndex);
		clearSelectedEntity();
		ImGui::End();
		return;
	}

	ImGui::SameLine(65);

	if (ImGui::Button("go to"))
	{
		std::shared_ptr<SceneCamera> camera = m_sceneHandle->getSceneCamera();
		camera->setPosition(glm::vec3(entity->GetXPos(), entity->GetYPos()+1, entity->GetZPos()));
	}


	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Pos X:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetXPos()).c_str());

		//ImGui::SameLine(sameLineSpacing);
		//float dab = 0.0f;
		//ImGui::InputFloat("dab", &dab);
		//entity->SetXPos(dab);

		ImGui::Text("Pos Y:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetYPos()).c_str());

		ImGui::Text("Pos Z:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetZPos()).c_str());

		ImGui::Separator();

		ImGui::Text("Rot X:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetXRot()).c_str());

		ImGui::Text("Rot Y:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetYRot()).c_str());

		ImGui::Text("Pos Z:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetZRot()).c_str());

		ImGui::Separator();

		ImGui::Text("Scale X:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetXScale()).c_str());

		ImGui::Text("Scale Y:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetYScale()).c_str());

		ImGui::Text("Scale Z:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(std::to_string(entity->GetZScale()).c_str());

		
	}

	if (ImGui::CollapsingHeader("Entity Texture"))
	{
		ImGui::Text(entity->getTextureAtSlot(e_diffuseTextureSlot)->getFilePath().c_str());
		ImGui::Image(reinterpret_cast<void*>(entity->getTextureAtSlot(e_diffuseTextureSlot)->getTextureID()), ImVec2(128, 128));

		ImGui::Separator();

		ImGui::Text(entity->getTextureAtSlot(e_specularTextureSlot)->getFilePath().c_str());
		ImGui::Image(reinterpret_cast<void*>(entity->getTextureAtSlot(e_specularTextureSlot)->getTextureID()), ImVec2(128, 128));
	}


	ImGui::End();
}

/// <summary>
/// Function is called every time the current scene changes. It updates the available buttons light buttons depending on what lights the scene has
/// </summary>
void UI::updateSceneInformation()
{
	// Get the new scene light manager
	std::shared_ptr<SceneLightManager> sceneLM = m_sceneHandle->getSceneLightManager();

	// Test whether there is a directionalLight in the scene
	m_directionalLightInScene = sceneLM->getCurrentDirectionalLights() > 0 ? true : false;

	// Test whether there is a spotLight in the scene
	m_spotLightInScene = sceneLM->getCurrentSpotLights() > 0 ? true : false;

	// Test how many point lights are in the scene
	int pointLightNum = sceneLM->getCurrentPointLights();
	m_pointLightInScene[e_FirstPointLight] = pointLightNum >= 1 ? true : false;
	m_pointLightInScene[e_SecondPointLight] = pointLightNum >= 2 ? true : false;
	m_pointLightInScene[e_ThirdPointLight] = pointLightNum >= 3 ? true : false;
	m_pointLightInScene[e_FourthPointLight] = pointLightNum >= 4 ? true : false;

	clearSelectedEntity();

}

/// <summary>
/// Clears the information for the entityPanel()
/// </summary>
void UI::clearSelectedEntity()
{
	m_isEntitySelected = false;
	m_selectedEntityIndex = -1;
}
