#include "pch.h"
#include "Rendering/MeshManager.h"

bool MeshManager::createMesh(const std::string& meshID, const std::string& meshFilepath)
{
	// Check if texture with ID already exists
	if (m_meshMap.find(meshID) != m_meshMap.end())
		return false;

	//PRINT_TRACE("created texture with ID {0}", meshID);

	Mesh* pMesh = new Mesh(meshFilepath);
	m_meshMap.insert({ meshID, pMesh });

	return true;
}

Mesh* MeshManager::getMeshAtID(const std::string& meshID)
{
	return m_meshMap.find(meshID) != m_meshMap.end() ? m_meshMap.at(meshID) : nullptr;
}

void MeshManager::clearAllMeshes()
{
	for (const auto& [key, value] : m_meshMap)
	{
		delete value;
	}
	m_meshMap.clear();

}
