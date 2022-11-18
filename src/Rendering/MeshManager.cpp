#include "pch.h"
#include "Rendering/MeshManager.h"

bool MeshManager::addMesh(const std::string& meshID)
{
	// Check if texture with ID already exists
	if (m_meshMap.find(meshID) != m_meshMap.end())
		return false;

	std::string meshFilepath = "res/meshes/" + meshID + ".obj";

	std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();
	pMesh->parseMesh(meshFilepath);

	m_meshMap.insert({ meshID, std::move(pMesh) });

	return true;
}

void MeshManager::createAllMeshes()
{
	for (const auto& [key, value] : m_meshMap)
	{
		// If mesh isn't already created, create it
		if (!value->getIsCreated())
			value->createMesh();
	}
}

void MeshManager::bindMeshAtID(const std::string& meshID)
{
	if (m_meshMap.count(meshID))
	{
		m_meshMap.at(meshID)->bindMesh();
	}
}

void MeshManager::unbindMeshAtID(const std::string& meshID)
{
	if (m_meshMap.count(meshID))
	{
		m_meshMap.at(meshID)->unbindMesh();
	}
}

size_t MeshManager::getIndicesCountAtID(const std::string meshID)
{
	if (m_meshMap.count(meshID))
	{
		return m_meshMap.at(meshID)->getIndicesCount();
	}
	return 0;
}

void MeshManager::clearAllMeshes()
{
	m_meshMap.clear();
}
