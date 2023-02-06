#include "pch.h"
#include "UI/ModelDataPanel.h"

#include "Scene/Scene.h"
#include "Rendering/Model.h"
#include "UI/SceneModelsPanel.h"
#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/IResource.h"

#include "imgui.h"

/// <summary>
/// Cache system that ensures data about a model is fetched once when first selected
/// The panel will use this struct to print data about a model
/// Data is about the selected model from the SceneModels panel
/// </summary>
struct SelectedModelCache
{
	SelectedModelCache() { PRINT_INFO("cache created"); }
	~SelectedModelCache() { PRINT_INFO("cache destroyed"); }

	Vector3D position;
	Vector3D rotation;
	Vector3D scale;

	std::weak_ptr<Model> tempModel;

	std::string modelID;
	std::string meshID;
	std::string materialID;

	ResourceID materialTextureIDs[5];
	std::string materialTextureFilepaths[5];
	OpenGLIndex materialTextureOpenGLIDs[5]{ 0,0,0,0,0 };
};


ModelDataPanel::ModelDataPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, std::weak_ptr<SceneModelsPanel> pSceneModelPanel)
	:IPanel(panelName, imGuiWindowFlag, bVisible), m_pSceneModelPanel(pSceneModelPanel), m_selectedModelIndex(-1)
{
}

ModelDataPanel::~ModelDataPanel()
{
}

/// <summary>
/// Inherited from IPanel
/// Checks and updates the ModelCache if a new model has been selected in SceneModelsPanel
/// </summary>
void ModelDataPanel::Update()
{
	if (m_pSceneModelPanel.lock()->GetUpdatedSelectedModel())
	{
		// Store index of model in Scene SceneModels vector
		m_selectedModelIndex = m_pSceneModelPanel.lock()->GetSelectedModelIndex();

		// Clear existing values of cache and setup new one
		if (m_pModelCache)
			ClearModelCache();
		SetupModelCache();

		m_bVisible = true;
	}
}

/// <summary>
/// Inherited from IPanel
/// Displays the data from SelectedModelCache which is data about the currently selected model
/// </summary>
void ModelDataPanel::Render()
{
	// Displays the model ID
	ImGui::Text("Model ID: %s", m_pModelCache->modelID.c_str());

	ImGui::Separator();

	// Display the buttons
	if (ImGui::Button("goto model"))
		GotoModel_BUTTON(m_pModelCache->tempModel.lock()->GetPosition());

	ImGui::SameLine(90.5f);

	if (ImGui::Button("delete model"))
	{
		DeleteModel_BUTTON();
		return;
	}

	ImGui::SameLine(187.5f);

	if (ImGui::Button("close panel"))
	{
		ClosePanel_BUTTON();
		return;
	}
		
	ImGui::Separator();

	static bool bTransformHeaderState = true;
	ImGui::SetNextTreeNodeOpen(bTransformHeaderState);
	if (bTransformHeaderState = ImGui::CollapsingHeader("Transform"))
	{
		// Display the models transform data
		ImGui::Text("Position X: %.2f Y: %.2f Z: %.2f ", m_pModelCache->position.GetX(), m_pModelCache->position.GetY(), m_pModelCache->position.GetZ());
		ImGui::Text("Rotation X: %.2f  Y: %.2f  Z: %.2f ", m_pModelCache->rotation.GetX(), m_pModelCache->rotation.GetY(), m_pModelCache->rotation.GetZ());
		ImGui::Text("Scale	X: %.2f  Y: %.2f  Z: %.2f ", m_pModelCache->scale.GetX(), m_pModelCache->scale.GetY(), m_pModelCache->scale.GetZ());
	}

	ImGui::Separator();

	static bool bMeshHeaderState = true;
	ImGui::SetNextTreeNodeOpen(bMeshHeaderState);
	if (bMeshHeaderState = ImGui::CollapsingHeader("Mesh"))
	{
		// Display the models mesh ID
		ImGui::Text("Mesh ID: %s", m_pModelCache->meshID.c_str());
	}

	ImGui::Separator();

	static bool bMaterialHeaderState = true;
	ImGui::SetNextTreeNodeOpen(bMaterialHeaderState);
	if (bMaterialHeaderState = ImGui::CollapsingHeader("Material"))
	{
		// Display the models material ID
		ImGui::Text("Material ID: %s", m_pModelCache->materialID.c_str());

		// Display the ID, filepath and image of each texture the material the model uses
		for (unsigned int i = 0; i < 5; i++)
		{
			if (m_pModelCache->materialTextureIDs[i] != EMPTY_TEXTURE_ID)
			{
				ImGui::Text("Texture ID: %s", m_pModelCache->materialTextureIDs[i].c_str());
				ImGui::Text("Texture Filepath: %s", m_pModelCache->materialTextureFilepaths[i].c_str());
				ImGui::Image((ImTextureID)m_pModelCache->materialTextureOpenGLIDs[i], ImVec2(128, 128));

				ImGui::Separator();
			}
		}
	}
}

/// <summary>
/// Inherited from IPanel
/// Deletes the current SelectedModelCache data
/// </summary>
void ModelDataPanel::SceneChange()
{
	ClearModelCache();
}

/// <summary>
/// Set all the values of the SelectedModelCache
/// Data is of the currently selected model from SceneModels panel
/// </summary>
void ModelDataPanel::SetupModelCache()
{
	m_pModelCache = std::make_unique<SelectedModelCache>();

	m_pModelCache->tempModel = m_pSceneHandle.lock()->GetModelAtIndex(m_selectedModelIndex).lock();

	m_pModelCache->modelID = m_pModelCache->tempModel.lock()->getModelID();

	m_pModelCache->position = m_pModelCache->tempModel.lock()->GetPosition();
	m_pModelCache->rotation = m_pModelCache->tempModel.lock()->GetRotation();
	m_pModelCache->scale = m_pModelCache->tempModel.lock()->GetScale();

	m_pModelCache->meshID = m_pModelCache->tempModel.lock()->GetMeshID();

	m_pModelCache->materialID = m_pModelCache->tempModel.lock()->GetMaterialID();

	auto tempMaterial = MaterialManager::Instance()->GetMaterialAtID(m_pModelCache->tempModel.lock()->GetMaterialID()).lock();
	auto tempMaterialTexturesIDs = tempMaterial->GetAllTextureIDs();

	for (unsigned int i = 0; i < 5; i++)
	{
		if (tempMaterialTexturesIDs.at(i) != EMPTY_TEXTURE_ID)
		{
			m_pModelCache->materialTextureIDs[i] = TextureManager::Get()->GetResourceAtID(tempMaterialTexturesIDs.at(i))->GetResourceID();
			m_pModelCache->materialTextureFilepaths[i] = TextureManager::Get()->GetResourceAtID(tempMaterialTexturesIDs.at(i))->GetFilepath();
			m_pModelCache->materialTextureOpenGLIDs[i] = TextureManager::Get()->GetResourceAtID(tempMaterialTexturesIDs.at(i))->GetOpenGLID();
		}
		else
		{
			m_pModelCache->materialTextureIDs[i] = EMPTY_TEXTURE_ID;
		}
	}
}

/// <summary>
/// Delete the SelectedModelsCache
/// </summary>
void ModelDataPanel::ClearModelCache()
{
	m_bVisible = false;

	if (m_pModelCache)
		m_pModelCache.reset();	
}

/// <summary>
/// Called when the "delete model" button is pressed in Model Data panel
/// Clears the data in the SelectedModelCache and deletes the model from the scene
/// </summary>
void ModelDataPanel::DeleteModel_BUTTON()
{
	// Delete model
	m_pSceneHandle.lock()->DeleteModelAtIndex(m_selectedModelIndex);

	// Delete any stored data of model
	ClearModelCache();
	m_selectedModelIndex = -1;
	m_pSceneModelPanel.lock()->ClearSelectedModelIndex();
}

/// <summary>
/// Called when the "goto model" button is pressed in Model Data panel
/// Moves the scene camera to the position of the model that is selected
/// </summary>
void ModelDataPanel::GotoModel_BUTTON(const Vector3D& newPos)
{
	// Move camera to slightly above model
	Vector3D temp{ newPos.GetX(), newPos.GetY() + 2.0f, newPos.GetZ() };

	m_pSceneHandle.lock()->SetSceneCameraPosition(temp);
}

/// <summary>
/// Called when the "close panel" button is pressed in Model Data panel
/// Clears the data in cache and turns off visibility of Model Data panel
/// </summary>
void ModelDataPanel::ClosePanel_BUTTON()
{
	ClearModelCache();
	m_selectedModelIndex = -1;
	m_pSceneModelPanel.lock()->ClearSelectedModelIndex();
}
