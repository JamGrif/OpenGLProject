#pragma once

struct Vertex;

/// <summary>
/// Encapsulates an OpenGL VBO and EBO buffer
/// </summary>
class Mesh
{
public:
	Mesh();
	~Mesh();

	size_t	GetIndicesCount() const { return m_meshIndices.size(); }

private:

	void BindMesh();
	void UnbindMesh();

	void ParseMesh(const std::string& filepath);
	void CreateMesh();

	bool GetIsCreated() { return m_bIsCreated; }

private:

	uint32_t							m_meshVBO;
	uint32_t							m_meshEBO;

	// All the vertices that make up the mesh
	std::vector<Vertex>					m_meshVertices;
	std::vector<unsigned int>			m_meshIndices;

	std::string							m_meshFilePath;

	bool	m_bIsCreated;

	friend class MeshManager;
};

