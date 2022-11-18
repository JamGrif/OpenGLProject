#include "pch.h"
#include "Rendering/MaterialManager.h"

bool MaterialManager::addMaterial(const std::string& materialID, const MaterialLoaderParams& pParams)
{
	// Check if material with ID already exists
	if (m_materialMap.find(materialID) != m_materialMap.end())
		return false;

	m_materialMap.insert({ materialID, std::make_unique<Material>(pParams) });

	return true;
}

void MaterialManager::setAllMaterialPointers(SceneLightManager* pSceneLightManager, SceneCamera* pSceneCamera, const glm::mat4& projMat)
{
	for (const auto& [key, value] : m_materialMap)
	{
		value->setScenePointers(pSceneLightManager, pSceneCamera, projMat);
	}
}

void MaterialManager::bindMaterialAtID(const std::string& materialID, const glm::mat4& modelMat)
{
	if (m_materialMap.count(materialID))
	{
		m_materialMap.at(materialID)->bindMaterial(modelMat);
	}
}

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
