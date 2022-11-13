#include "pch.h"
#include "Rendering/MaterialManager.h"

bool MaterialManager::createMaterial(const std::string materialID, const MaterialLoaderParams& pParams)
{
	// Check if material with ID already exists
	if (m_materialMap.find(materialID) != m_materialMap.end())
		return false;

	//PRINT_TRACE("created material with ID {0}", materialID);

	Material* pMaterial = new Material(pParams);
	m_materialMap.insert({ materialID, pMaterial });

	return true;
}

Material* MaterialManager::getMaterialAtID(const std::string materialID)
{
	return m_materialMap.find(materialID) != m_materialMap.end() ? m_materialMap.at(materialID) : nullptr;
}

void MaterialManager::clearAllMaterials()
{
	//for (const auto& [key, value] : m_materialMap)
	//{
	//	delete value;
	//}
	m_materialMap.clear();
}
