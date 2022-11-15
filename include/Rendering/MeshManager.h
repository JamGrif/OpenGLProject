#pragma once

// Everything that uses MeshManager will use Mesh
#include "Rendering/Mesh.h"


class MeshManager
{
public:

	bool parseMesh(const std::string& meshID);

	void createAllMeshes();

	Mesh* getMeshAtID(const std::string& meshID);

	void clearAllMeshes();

	static MeshManager* Instance()
	{
		static MeshManager* s_pInstance = new MeshManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, Mesh*> m_meshMap;

	MeshManager() {}
	~MeshManager() {}

};
typedef MeshManager TheMeshManager;