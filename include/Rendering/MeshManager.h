#pragma once

// Everything that uses MeshManager will use Mesh
#include "Rendering/Mesh.h"

/// <summary>
/// Encapsulates and abstracts the creation, deletion and usage of Mesh objects
/// Meshes can only be used through this class
/// </summary>
class MeshManager
{
public:

	bool	AddMesh(const std::string& meshID);
	void	CreateAllMeshes();

	void	BindMeshAtID(const std::string& meshID);
	void	UnbindMeshAtID(const std::string& meshID);

	size_t	GetIndicesCountAtID(const std::string& meshID);

	void	ClearAllMeshes();

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
