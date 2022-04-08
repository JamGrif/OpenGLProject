#pragma once

// Forward Declarations
struct Vertex;

/// <summary>
/// Stores mesh information and provides a way to load a mesh by providing an interface for MeshManager
/// </summary>
class OpenGLMesh
{
public:
	OpenGLMesh();
	~OpenGLMesh();

	// Drawing functions
	void								Bind() const;
	void								Unbind() const;

	void								setVertexAttributes(bool shaderPos, bool shaderNorm, bool shaderTex, bool shaderTanBi);

	const std::vector<unsigned int>&	getIndices() const;

	// Mesh loading functions
	inline bool							readMeshFromFile();
	inline void							loadMesh();

	inline void							setFilePath(const std::string& filePath);
	inline const std::string&			getFilePath() const;

	int getMeshNumVertices() {return m_meshNumVertices;}
	
private:

	uint32_t							m_meshVBO;
	uint32_t							m_meshEBO;

	void								bindVertexAttributes() const;

	bool								m_vertexAttributesEnabled[4];

	int									m_meshNumVertices;
	//aiMesh*							m_meshForCollision;

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

	static std::shared_ptr<OpenGLMesh>	retrieveMeshObject(const std::string& filePath);

	static void						readMeshesFromFile();
	static void						createMeshes();

	static void						clearMeshes();

private:

	static std::vector<std::shared_ptr<OpenGLMesh>>	m_loadedMeshes;

	MeshManager(); //Private so a class of this can't be initialized
};
