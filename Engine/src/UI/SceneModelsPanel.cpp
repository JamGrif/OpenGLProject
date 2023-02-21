#include "pch.h"
#include "UI/SceneModelsPanel.h"

#include "imgui.h"

#include "Scene/Scene.h"
#include "Rendering/Model.h"

SceneModelsPanel::SceneModelsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer)
	:IPanel(panelName, imGuiWindowFlag, bVisible, pScenePointer), m_selectedModelIndex(NO_MODEL_SELECTED), m_bNewSelectedModel(false)
{
}

SceneModelsPanel::~SceneModelsPanel()
{
}

/// <summary>
/// Inherited from IPanel
/// Unused in this class
/// </summary>
void SceneModelsPanel::Update()
{
}

/// <summary>
/// Inherited from IPanel
/// Displays all available models in loaded scene and stores which model button was pressed for ModelDataPanel
/// </summary>
void SceneModelsPanel::Render()
{
	// Get current amount of models in scene
	unsigned int numOfModels = static_cast<unsigned int>((*m_pSceneHandle)->GetNumberOfModels());

	ImGui::Text("There are %i models in scene", numOfModels);

	m_bNewSelectedModel = false;

	// Display button for each model in scene
	for (unsigned int i = 0; i < numOfModels; i++)
	{
		if (ImGui::Button((*m_pSceneHandle)->GetModelAtIndex(i).lock()->GetModelID().c_str()))
		{
			// Stops reloading same model data
			if (m_selectedModelIndex == i)
				continue;

			m_selectedModelIndex = i;
			m_bNewSelectedModel = true;
		}
	}
}

/// <summary>
/// Inherited from IPanel
/// Resets the selected model on scene change
/// </summary>
void SceneModelsPanel::SceneChange()
{
	m_selectedModelIndex = NO_MODEL_SELECTED;
	m_bNewSelectedModel = false;
}
