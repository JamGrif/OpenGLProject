#pragma once

#include "Rendering/Resource/IResource.h"

struct Vertex;

/// <summary>
/// Encapsulates an OpenGL VBO and EBO buffer
/// </summary>
class Mesh :
	public IResource
{
public:
	Mesh();
	~Mesh();

	size_t	GetIndicesCount() const { return m_meshIndices.size(); }

 private:

	virtual void Bind() override;
	virtual void Unbind() override;

	virtual void Parse(const std::string& filepath) override;
	virtual void Parse(const std::string& firstFilepath, const std::string& secondFilepath) {}

	virtual void Create() override;

	virtual void Reset() override;

	//bool GetCreated() { return m_bIsCreated; }

private:

	//uint32_t							m_meshVBO;
	uint32_t							m_meshEBO;

	// All the vertices that make up the mesh
	std::vector<Vertex>					m_meshVertices;
	std::vector<unsigned int>			m_meshIndices;

	//std::string							m_meshFilePath;

	//bool	m_bIsCreated;

	template<typename Mesh>
	friend class ResourceManager;
};

