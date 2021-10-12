#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

//Stores mesh information and provides a way to load a mesh by interfacing with MeshManager
class Mesh
{
public:
	Mesh();
	~Mesh();

	bool						loadMesh(const char* filePath);

	std::vector<Vertex>			getVertices() const;
	std::vector<unsigned int>	getIndices() const;
	const char*					getFilePath() const;

private:

	const char*					m_filePath;

	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	m_indices;

};

//Stores current loaded meshes ensuring only one of each mesh gets loaded. Provides a way to load a mesh from a file
class MeshManager
{
public:

	static Mesh*				loadModel(const char* filePath);

	static void					clearMeshes();

private:

	static std::vector<Mesh*>	m_loadedModels;

	MeshManager(); //Private so a class of this can't be initalized
};

