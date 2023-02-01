#pragma once

#include "Rendering/Resource/IResource.h"



/// <summary>
/// Encapsulates an OpenGL VBO and EBO buffer
/// </summary>
class Mesh :
	public IResource
{
public:
	Mesh();
	~Mesh();

	size_t			GetIndicesCount() const { return m_meshIndices.size(); }

 private:

	 virtual void	Bind() override;
	 virtual void	Unbind() override;

	 virtual void	Parse(const std::string& filepath) override;
	 virtual void	Parse(const std::string& firstFilepath, const std::string& secondFilepath) override {}

	 virtual void	Create() override;

	 virtual void	Reset() override;

private:

	uint32_t		m_meshEBO;

	MeshVertices	m_meshVertices;
	MeshIndices		m_meshIndices;

	template<typename Mesh>
	friend class ResourceManager;
};

