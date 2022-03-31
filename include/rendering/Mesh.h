#pragma once

#include "rendering/BaseAsset.h"

// Forward Declarations
struct Vertex;

/// <summary>
/// Stores mesh information and provides a way to load a mesh by providing an interface for MeshManager
/// </summary>
class Mesh :
	public BaseAsset
{
public:
	Mesh();
	~Mesh();

	// Drawing functions
	void								Bind(bool shaderPos = false, bool shaderNorm = false, bool shaderTex = false, bool shaderTanBi = false) const;
	void								Unbind() const;

	const std::vector<unsigned int>&	getIndices() const;

	// Mesh loading functions
	inline bool							readMeshFromFile();
	inline void							loadMesh();

	inline void							setFilePath(const std::string& filePath);
	inline const std::string&			getFilePath() const;

	int getMeshNumVertices() {return m_meshNumVertices;}
	

private:

	GLuint								m_meshVBO;
	GLuint								m_meshEBO;

	int									m_meshNumVertices;
	//aiMesh*								m_meshForCollision;

	std::vector<Vertex>					m_meshVertices;
	std::vector<unsigned int>			m_meshIndices;

	std::string							m_filePath;

};

/// <summary>
/// Stores created mesh objects, only one version of a mesh object will exist for each filepath set.
///	Do all mesh creation and deleting through the MeshManager
/// </summary>
class MeshManager
{
public:

	static std::shared_ptr<Mesh>	retrieveMeshObject(const std::string& filePath);

	static void						readMeshesFromFile();
	static void						createMeshes();

	static void						clearMeshes();

private:

	static std::vector<std::shared_ptr<Mesh>>	m_loadedMeshes;

	MeshManager(); //Private so a class of this can't be initialized
};
