#include "pch.h"
#include "Rendering/MeshManager.h"

static const std::string FILEPATH_PREFIX = "res/meshes/";
static const std::string FILEPATH_SUFFIX = ".obj";

/// <summary>
/// Parse the specified mesh and add it to the meshMap
/// IMPORTANT: Name of mesh file must be the same as its meshID
/// IMPORTANT: When all meshes have been parsed, CreateAllMeshes() must be called before meshes can be used
/// </summary>
bool MeshManager::addMesh(const std::string& meshID)
{
	// Check if texture with ID already exists
	if (m_meshMap.find(meshID) != m_meshMap.end())
		return false;

	// Automatically set the filepath of mesh
	std::string meshFilepath = FILEPATH_PREFIX + meshID + FILEPATH_SUFFIX;

	// Create and parse the mesh file
	std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();
	pMesh->parseMesh(meshFilepath);

	m_meshMap.insert({ meshID, std::move(pMesh) });

	return true;
}

/// <summary>
/// Create all meshes that have been successfully parsed
/// </summary>
void MeshManager::createAllMeshes()
{
	for (const auto& [key, value] : m_meshMap)
	{
		// If mesh isn't already created, create it
		if (!value->getIsCreated())
			value->createMesh();
	}
}

/// <summary>
/// Binds the specified mesh to the OpenGL state
/// </summary>
void MeshManager::bindMeshAtID(const std::string& meshID)
{
	if (m_meshMap.count(meshID))
	{
		m_meshMap.at(meshID)->bindMesh();
	}
}

/// <summary>
/// Unbinds the specified mesh to the OpenGL state
/// </summary>
void MeshManager::unbindMeshAtID(const std::string& meshID)
{
	if (m_meshMap.count(meshID))
	{
		m_meshMap.at(meshID)->unbindMesh();
	}
}

/// <summary>
/// Returns the number of indices that the specified mesh has
/// Used for the draw call to OpenGL
/// </summary>
size_t MeshManager::getIndicesCountAtID(const std::string meshID)
{
	if (m_meshMap.count(meshID))
	{
		return m_meshMap.at(meshID)->getIndicesCount();
	}
	return 0;
}

/// <summary>
/// Deletes all meshes in the meshMap
/// </summary>
void MeshManager::clearAllMeshes()
{
	m_meshMap.clear();
}
