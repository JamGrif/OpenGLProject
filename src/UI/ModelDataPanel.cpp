#include "pch.h"
#include "UI/ModelDataPanel.h"

#include "Scene/Scene.h"
#include "Rendering/Model.h"
#include "UI/SceneModelsPanel.h"

#include "imgui.h"


ModelDataPanel::ModelDataPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, SceneModelsPanel* pSceneModelPanel)
	:IPanel(panelName, imGuiWindowFlag), m_pSceneModelPanel(pSceneModelPanel), m_bVisible(false)
{

}

ModelDataPanel::~ModelDataPanel()
{

}

void ModelDataPanel::Render()
{
	if (m_pSceneModelPanel->GetSelectedModelIndex() != -1)
	{
		m_bVisible = true;
		m_selectedModelIndex = m_pSceneModelPanel->GetSelectedModelIndex();
	}

	if (!m_bVisible)
		return;


	StartOfFrame();

	std::shared_ptr<Model> tempModel = m_sceneHandle.lock()->getModelAtIndex(m_selectedModelIndex);

	ImGui::Text(tempModel->getModelID().c_str());

	ImGui::Separator();

	ImGui::Text(tempModel->getModelID().c_str());

	EndOfFrame();
}

void ModelDataPanel::SceneChange()
{
	ClearSelectedEntity();
}

void ModelDataPanel::ClearSelectedEntity()
{
	m_bVisible = false;
	m_selectedModelIndex = -1;
}
