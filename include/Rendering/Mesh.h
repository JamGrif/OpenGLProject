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

private:

	void bindMesh();
	void unbindMesh();

	void parseMesh(const std::string& filepath);
	void createMesh();

	size_t getIndicesCount() const { return m_meshIndices.size(); }
	bool	getIsCreated() { return m_bIsCreated; }

private:

	uint32_t							m_meshOpenGLVBO;
	uint32_t							m_meshOpenGLEBO;

	// All the vertices that make up the mesh
	std::vector<Vertex>					m_meshVertices;
	std::vector<unsigned int>			m_meshIndices;

	std::string							m_meshFilePath;

	bool	m_bIsCreated;

	friend class MeshManager;
};

