#pragma once

struct Vertex;

class Mesh
{
public:
	Mesh();
	~Mesh();

	void parseMesh(const std::string& filepath);
	void createMesh();

	void bindMesh();
	void unbindMesh();

	const std::vector<unsigned int>& getIndices() const;


private:

	uint32_t							m_meshVBO = 0;
	uint32_t							m_meshEBO = 0;

	bool								m_vertexAttributesEnabled[4];

	std::vector<Vertex>					m_meshVertices;
	std::vector<unsigned int>			m_meshIndices;

	std::string							m_filePath;


};

