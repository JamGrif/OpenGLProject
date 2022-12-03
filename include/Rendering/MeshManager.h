#pragma once

// Everything that uses MeshManager will use Mesh
#include "Rendering/Mesh.h"

/// <summary>
/// Abstracts and handles the creation, deletion and usage of Mesh objects
/// Meshes can only be used through this class
/// </summary>
class MeshManager
{
public:

	bool	addMesh(const std::string& meshID);

	void	createAllMeshes();

	void	bindMeshAtID(const std::string& meshID);
	void	unbindMeshAtID(const std::string& meshID);

	size_t	getIndicesCountAtID(const std::string meshID);

	void	clearAllMeshes();

	static MeshManager* Instance() // Get instance
	{
		static MeshManager* s_pInstance = new MeshManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshMap;

	MeshManager() {}
	~MeshManager() {}
	MeshManager(const MeshManager&) = delete;
};
typedef MeshManager TheMeshManager;
