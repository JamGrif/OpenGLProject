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

// Flags for each ImGui window used
static ImGuiWindowFlags commonResizeFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
static ImGuiWindowFlags commonFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
static ImGuiWindowFlags debugFlags = ImGuiWindowFlags_NoCollapse;

/// <summary>
/// This allows the properties of the selected entity to be found once and stored to allow faster retrieval of the selected entity information in the entity panel
/// If a property of the entity gets updated through the editor, the stored entity values will get refreshed to reflect any changes
/// </summary>
struct selectedEntityCache
{
	selectedEntityCache(std::shared_ptr<LightingEntity> entity)
		:entityPtr(entity)
	{
		//PRINT_TRACE("created cached data");

		entityType = entityPtr->getEntityType();
		editorName = entityPtr->getEditorName();

		refreshTransformCachedData();
		refreshMeshCachedData();
		refreshTextureCachedData();
	}
	~selectedEntityCache()
	{
		//PRINT_TRACE("deleted cached data");
	}

	std::string entityType; // LightingEntity, BasicEntity, SkyEntity etc...
	std::string editorName;	// Name in the Scene Entities panel

	// Transform data stored as strings to allow easy use with ImGui
	std::string posX;
	std::string posY;
	std::string posZ;

	std::string rotX;
	std::string rotY;
	std::string rotZ;

	std::string scaleX;
	std::string scaleY;
	std::string scaleZ;

	std::string meshFilepath;

	std::size_t totalTextures;
	std::string textureFilepath[5];
	uint32_t	textureOpenGLID[5];

	inline void checkCachedDataForUpdate()
	{
		// If the transform part of the selected entity has been changed then refresh the entity transform values in the editor
		if (entityPtr->getTransformUpdated())
		{
			refreshTransformCachedData();
		}

		// check and refresh mesh data

		// check and refresh texture data

	}

	// Only updates the selected objects transform data
	void refreshTransformCachedData()
	{
		posX = std::to_string(entityPtr->GetXPos());
		posY = std::to_string(entityPtr->GetYPos());
		posZ = std::to_string(entityPtr->GetZPos());

		rotX = std::to_string(entityPtr->GetXRot());
		rotY = std::to_string(entityPtr->GetYRot());
		rotZ = std::to_string(entityPtr->GetZRot());

		scaleX = std::to_string(entityPtr->GetXScale());
		scaleY = std::to_string(entityPtr->GetYScale());
		scaleZ = std::to_string(entityPtr->GetZScale());
	}

	// Only updates the selected objects mesh data
	void refreshMeshCachedData()
	{
		meshFilepath = entityPtr->getMesh()->getFilePath();
	}

	// Only updates the selected objects texture data
	void refreshTextureCachedData()
	{
		totalTextures = entityPtr->getTextureAmount();

		for (int i = 0; i < totalTextures; i++)
		{
			textureFilepath[i] = entityPtr->getTextureAtSlot(i)->getFilePath().c_str();
			textureOpenGLID[i] = entityPtr->getTextureAtSlot(i)->getTextureID();
		}
	}

private:
	std::shared_ptr<LightingEntity> entityPtr; // Pointer to the actual entity
};

UI::UI(bool uiVisible, std::shared_ptr<Scene> loadedScene)
	:m_isEntitySelected(false), m_selectedEntityIndex(-1), m_uiVisible(uiVisible), m_sceneHandle(loadedScene), m_sceneNum(0),
	m_directionalLightInScene(false), m_directionalLightActiveButton(true),
	m_spotLightInScene(false), m_spotLightActiveButton(true),
	m_totalPointLights(0), m_pointLightInScene{ false, false, false, false }, m_pointLightActiveButton{ true, true, true, true },
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

	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowRounding = 5.0f;
	style->FrameRounding = 4.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;

	// Connect ImGui to GLFW window
	ImGui_ImplGlfw_InitForOpenGL(EngineStatics::getAppWindow()->getRaw(), true);
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
/// Called at start of each frame, before UI drawInFrame()
/// Gets ImGui ready for this frame
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
	m_uiVisible = !m_uiVisible;

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
/// Updates the cached pointer to the currently loaded scene
/// </summary>
/// <param name="newLoadedScene">Pointer to the new scene</param>
void UI::updateSceneHandle(std::shared_ptr<Scene> newLoadedScene)
{
	m_sceneHandle = newLoadedScene;

	// If newLoadedScene was set to be null then don't update any information
	if (!newLoadedScene)
		return;

	updateSceneInformation();
}

/// <summary>
/// Renders the Scene Options ImGui window
/// </summary>
void UI::sceneOptionsPanel()
{
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

	for (int i = 0; i < m_totalPointLights; i++)
	{
		if (m_pointLightInScene[i])
		{
			std::string nameTemp = "PointLight" + std::to_string(i);
			ImGui::Checkbox(nameTemp.c_str(), &m_pointLightActiveButton[i]);

			// Set the active state of the pointLight depending on the check box status
			sceneLM->getPointLight(i)->lightActive = m_pointLightActiveButton[i] ? true : false;
		}
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

/// <summary>
/// Renders the Controls ImGui window
/// </summary>
void UI::controlsPanel()
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
void UI::performanceMetricsPanel()
{
	std::string fps = "FPS: " + std::to_string(ApplicationClock::getFrameCount());
	std::string delta = "Delta Time: " + std::to_string(ApplicationClock::getDeltaTime());

	ImGui::Begin("Performance Metrics:", NULL, commonResizeFlags);

	ImGui::Text(fps.c_str());
	ImGui::Text(delta.c_str());

	ImGui::End();
}

/// <summary>
/// Renders the Scene Entities ImGui window
/// </summary>
void UI::sceneEntitiesPanel()
{
	ImGui::Begin("Scene Entities:", NULL, commonFlags);
	for (int i = 0; i < m_sceneHandle->getEntityNum(); i++)
	{
		//std::string buttonName = m_sceneHandle->getEntityAtIndex(i)->getEntityType() + std::to_string(i);
		std::string buttonName = m_sceneHandle->getEntityAtIndex(i)->getEditorName();
		if (ImGui::Button(buttonName.c_str()))
		{
			if (i != m_selectedEntityIndex)
			{
				// First, clear any information on the currently selected entity
				clearSelectedEntity();

				// Now cache the information on the selected entity
				m_selectedEntityIndex = i;
				m_isEntitySelected = true;
				m_selectedEntity = std::make_unique<selectedEntityCache>(m_sceneHandle->getEntityAtIndex(i));
			}
		}
	}
	
	ImGui::End();
}

/// <summary>
/// Renders the Entity ImGui window
/// </summary>
void UI::entityPanel()
{
	// Checks each component of the selected entity to check if any of the cached data needs to be updated
	m_selectedEntity->checkCachedDataForUpdate();

	ImGui::Begin("Entity:", NULL, commonFlags);

	ImGui::Text("Entity Name:");
	ImGui::SameLine(95);
	ImGui::Text(m_selectedEntity->editorName.c_str());

	ImGui::Text("Entity Type:");
	ImGui::SameLine(95);
	ImGui::Text(m_selectedEntity->entityType.c_str());

	if (ImGui::Button("delete")) // Delete entity
	{
		m_sceneHandle->deleteLightingEntityFromVector(m_selectedEntityIndex);
		clearSelectedEntity();
		ImGui::End();
		return;
	}

	ImGui::SameLine(65);

	if (ImGui::Button("go to")) // Moves the camera to the entities position
	{
		std::shared_ptr<SceneCamera> camera = m_sceneHandle->getSceneCamera();
		camera->setPosition(glm::vec3(std::stoi(m_selectedEntity->posX), std::stoi(m_selectedEntity->posY)+1, std::stoi(m_selectedEntity->posZ)));
	}

	ImGui::SameLine(114);

	if (ImGui::Button("close")) // Closes the entity panel
	{
		clearSelectedEntity();
		ImGui::End();
		return;
	}

	constexpr int sameLineSpacing = 70;
	static bool transformHeaderState = true;
	ImGui::SetNextTreeNodeOpen(transformHeaderState);
	if (transformHeaderState = ImGui::CollapsingHeader("Entity Transform"))
	{
		ImGui::Text("Pos X:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->posX.c_str());

		ImGui::Text("Pos Y:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->posY.c_str());

		ImGui::Text("Pos Z:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->posZ.c_str());

		ImGui::Separator();

		ImGui::Text("Rot X:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->rotX.c_str());

		ImGui::Text("Rot Y:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->rotY.c_str());

		ImGui::Text("Pos Z:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->rotZ.c_str());

		ImGui::Separator();

		ImGui::Text("Scale X:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->scaleX.c_str());

		ImGui::Text("Scale Y:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->scaleY.c_str());

		ImGui::Text("Scale Z:");
		ImGui::SameLine(sameLineSpacing);
		ImGui::Text(m_selectedEntity->posZ.c_str());
	}

	static bool meshHeaderState = true;
	ImGui::SetNextTreeNodeOpen(meshHeaderState);
	if (meshHeaderState = ImGui::CollapsingHeader("Entity Mesh"))
	{
		ImGui::Text(m_selectedEntity->meshFilepath.c_str());
	}

	static bool textureHeaderState = true;
	ImGui::SetNextTreeNodeOpen(textureHeaderState);
	if (textureHeaderState = ImGui::CollapsingHeader("Entity Texture"))
	{
		for (int i = 0; i < m_selectedEntity->totalTextures; i++)
		{
			ImGui::Text(m_selectedEntity->textureFilepath[i].c_str());
			ImGui::Image(reinterpret_cast<void*>(m_selectedEntity->textureOpenGLID[i]), ImVec2(128, 128));
			
			ImGui::Separator();
		}
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

	enum
	{
		e_FirstPointLight  = 0,
		e_SecondPointLight = 1,
		e_ThirdPointLight  = 2,
		e_FourthPointLight = 3
	};

	// Test how many point lights are in the scene
	m_totalPointLights = sceneLM->getCurrentPointLights();
	m_pointLightInScene[e_FirstPointLight] = m_totalPointLights >= 1 ? true : false;
	m_pointLightInScene[e_SecondPointLight] = m_totalPointLights >= 2 ? true : false;
	m_pointLightInScene[e_ThirdPointLight] = m_totalPointLights >= 3 ? true : false;
	m_pointLightInScene[e_FourthPointLight] = m_totalPointLights >= 4 ? true : false;

	clearSelectedEntity();
}

/// <summary>
/// Clears the information for the entityPanel()
/// </summary>
void UI::clearSelectedEntity()
{
	m_isEntitySelected = false;
	m_selectedEntityIndex = -1;
	m_selectedEntity = nullptr;
}
