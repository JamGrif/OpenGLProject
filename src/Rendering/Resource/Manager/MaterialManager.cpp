#include "pch.h"
#include "Rendering/Resource/Manager/MaterialManager.h"

/// <summary>
/// Create a new material using the values in the MaterialLoaderParams and add it to the materialMap at ID
/// </summary>
bool MaterialManager::CreateMaterial(const std::string& materialID, const MaterialLoaderParams& pParams)
{
	// Check if material with ID already exists
	if (m_materialMap.find(materialID) != m_materialMap.end())
		return false;

	// Create material and insert into materialMap
	m_materialMap.insert({ materialID, std::make_shared<Material>(pParams) });

	return true;
}

/// <summary>
/// Binds the textures and shaders used by the specified material to the OpenGL context
/// </summary>
void MaterialManager::BindMaterialAtID(const std::string& materialID, const glm::mat4& modelMat)
{
	if (m_materialMap.count(materialID))
	{
		m_materialMap.at(materialID)->BindMaterial(modelMat);
	}
}

/// <summary>
/// Unbinds the textures and shaders used by the specified material to the OpenGL context
/// </summary>
void MaterialManager::UnbindMaterialAtID(const std::string& materialID)
{
	if (m_materialMap.count(materialID))
	{
		m_materialMap.at(materialID)->UnbindMaterial();
	}
}

/// <summary>
/// Deletes all materials
/// </summary>
void MaterialManager::ClearAllMaterials()
{
	m_materialMap.clear();
}

/// <summary>
/// Sets the scene pointers for all materials
/// </summary>
void MaterialManager::SetAllMaterialScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera)
{
	for (const auto& [materialID, material] : m_materialMap)
	{
		material->SetScenePointers(pSceneLightManager, pSceneCamera);
	}
}
