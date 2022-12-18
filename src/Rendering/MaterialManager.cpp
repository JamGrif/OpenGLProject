#include "pch.h"
#include "Rendering/MaterialManager.h"

/// <summary>
/// Create a new material using the values in the MaterialLoaderParams and add it to the materialMap at ID
/// </summary>
bool MaterialManager::createMaterial(const std::string& materialID, const MaterialLoaderParams& pParams)
{
	// Check if material with ID already exists
	if (m_materialMap.find(materialID) != m_materialMap.end())
		return false;

	// Create material and insert into materialMap
	m_materialMap.insert({ materialID, std::make_unique<Material>(pParams) });

	return true;
}

/// <summary>
/// Sets the pointers of the scene lightmanager, scene camera and renderers projection matrix for all materials in materialMap
/// </summary>
void MaterialManager::setAllMaterialPointers(SceneLightManager* pSceneLightManager, SceneCamera* pSceneCamera, const glm::mat4& projMat)
{
	for (const auto& [key, value] : m_materialMap)
	{
		value->setScenePointers(pSceneLightManager, pSceneCamera, projMat);
	}
}

/// <summary>
/// Binds the textures and shaders used by the specified material to the OpenGL context
/// </summary>
void MaterialManager::bindMaterialAtID(const std::string& materialID, const glm::mat4& modelMat)
{
	if (m_materialMap.count(materialID))
	{
		m_materialMap.at(materialID)->bindMaterial(modelMat);
	}
}

/// <summary>
/// Unbinds the textures and shaders used by the specified material to the OpenGL context
/// </summary>
void MaterialManager::unbindMaterialAtID(const std::string& materialID)
{
	if (m_materialMap.count(materialID))
	{
		m_materialMap.at(materialID)->unbindMaterial();
	}
}

void MaterialManager::clearAllMaterials()
{
	m_materialMap.clear();
}
