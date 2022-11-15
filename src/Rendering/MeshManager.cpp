#include "pch.h"
#include "Rendering/MeshManager.h"

bool MeshManager::parseMesh(const std::string& meshID)
{
	// Check if texture with ID already exists
	if (m_meshMap.find(meshID) != m_meshMap.end())
		return false;

	std::string meshFilepath = "res/meshes/" + meshID + ".obj";

	Mesh* pMesh = new Mesh;
	pMesh->parseMesh(meshFilepath);

	m_meshMap.insert({ meshID, pMesh });

	return true;
}

void MeshManager::createAllMeshes()
{
	for (const auto& [key, value] : m_meshMap)
	{
		value->createMesh();
	}
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
