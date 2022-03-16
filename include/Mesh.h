#pragma once

#include "glm/glm.hpp"

#include <vector>
#include <string>
#include <memory>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

// Stores mesh information and provides a way to load a mesh by interfacing with MeshManager
class Mesh
{
public:
	Mesh();
	~Mesh();

	bool								loadMesh(const std::string& filePath);

	const std::vector<Vertex>&			getVertices() const;
	const std::vector<unsigned int>&	getIndices() const;
	const std::string&					getFilePath() const;

private:

	std::string					m_filePath;

	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	m_indices;
};

//Stores current loaded meshes ensuring only one of each mesh gets loaded. Provides a way to load a mesh from a file
class MeshManager
{
public:

	static std::shared_ptr<Mesh>	loadMesh(const std::string& filePath);

	static void						clearMeshes();

private:

	static std::vector<std::shared_ptr<Mesh>>	m_loadedMeshes;

	MeshManager(); //Private so a class of this can't be initialized
};
