#include "pch.h"
#include "Rendering/MeshManager.h"

static const std::string MESH_FILEPATH_PREFIX = "res/meshes/";
static const std::string MESH_FILEPATH_SUFFIX = ".obj";

/// <summary>
/// Parse the specified mesh and add it to the meshMap
/// IMPORTANT: Name of mesh file must be the same as its meshID
/// IMPORTANT: When all meshes have been parsed, createAllMeshes() must be called before meshes can be used
/// </summary>
bool MeshManager::AddMesh(const std::string& meshID)
{
	// Check if mesh with ID already exists
	if (m_meshMap.find(meshID) != m_meshMap.end())
		return false;

	// Automatically set the filepath of mesh
	std::string meshFilepath = MESH_FILEPATH_PREFIX + meshID + MESH_FILEPATH_SUFFIX;

	// Create and parse the mesh file
	std::unique_ptr<Mesh> pMesh = std::make_unique<Mesh>();
	pMesh->ParseMesh(meshFilepath);

	m_meshMap.insert({ meshID, std::move(pMesh) });

	return true;
}

/// <summary>
/// Create all meshes that have been successfully parsed
/// </summary>
void MeshManager::CreateAllMeshes()
{
	for (const auto& [key, value] : m_meshMap)
	{
		// If mesh isn't already created, create it
		if (!value->GetIsCreated())
			value->CreateMesh();
	}
}

/// <summary>
/// Binds the specified mesh to the OpenGL state
/// </summary>
void MeshManager::BindMeshAtID(const std::string& meshID)
{
	if (m_meshMap.count(meshID))
	{
		m_meshMap.at(meshID)->BindMesh();
	}
}

/// <summary>
/// Unbinds the specified mesh to the OpenGL state
/// </summary>
void MeshManager::UnbindMeshAtID(const std::string& meshID)
{
	if (m_meshMap.count(meshID))
	{
		m_meshMap.at(meshID)->UnbindMesh();
	}
}

/// <summary>
/// Returns the number of indices that the specified mesh has
/// Used for the draw call to OpenGL
/// </summary>
size_t MeshManager::GetIndicesCountAtID(const std::string& meshID)
{
	if (m_meshMap.count(meshID))
	{
		return m_meshMap.at(meshID)->GetIndicesCount();
	}
	return 0;
}

/// <summary>
/// Deletes all meshes in the meshMap
/// </summary>
void MeshManager::ClearAllMeshes()
{
	m_meshMap.clear();
}
