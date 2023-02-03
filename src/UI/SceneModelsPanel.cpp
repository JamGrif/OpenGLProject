#include "pch.h"
#include "UI/SceneModelsPanel.h"

#include "imgui.h"

#include "Scene/Scene.h"
#include "Rendering/Model.h"

SceneModelsPanel::SceneModelsPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag)
	:IPanel(panelName, imGuiWindowFlag), m_selectedModelIndex(-1)
{
}

SceneModelsPanel::~SceneModelsPanel()
{
}

void SceneModelsPanel::Render()
{
	StartOfFrame();

	unsigned int numOfModels = m_sceneHandle.lock()->getNumberOfModels();

	ImGui::Text("There are %i models in scene", numOfModels);

	for (int i = 0; i < numOfModels; i++)
	{
		if (ImGui::Button(m_sceneHandle.lock()->getModelAtIndex(i)->getModelID().c_str()))
		{
			// Stops reloading same model data
			if (m_selectedModelIndex == i)
				continue;

			m_selectedModelIndex = i;
			PRINT_TRACE("{0}", m_selectedModelIndex);
		}
	}

	EndOfFrame();
}

void SceneModelsPanel::SceneChange()
{
	m_selectedModelIndex = -1;
}
