#include "pch.h"
#include "rendering/OpenGLMesh.h"

#include "Rendering/OpenGLErrorCheck.h"

#include <assimp/Importer.hpp>		// Importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post-processing flags

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <GL/glew.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

enum vertexAttributes
{
	e_ShaderPos = 0,
	e_ShaderNorm = 1,
	e_ShaderTex = 2,
	e_ShaderTanBi = 3
};

std::vector<std::shared_ptr<OpenGLMesh>> MeshManager::m_loadedMeshes;

OpenGLMesh::OpenGLMesh()
	:m_meshVBO(0), m_meshEBO(0), m_vertexAttributesEnabled{ false,false,false,false }, m_filePath("")
{
}

OpenGLMesh::~OpenGLMesh()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	glCall(glDeleteBuffers(1, &m_meshVBO));
	glCall(glDeleteBuffers(1, &m_meshEBO));
}

/// <summary>
/// Binds the meshes VBO and EBO and ONLY sets the position shader vertex attribute
/// Only used during the first pass of drawing a LightingEntity (to allow shadows)
/// </summary>
void OpenGLMesh::BindJustPos() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO));

	glCall(glEnableVertexAttribArray(0));
	glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
}

/// <summary>
/// Binds the meshes VBO and EBO and sets the shader vertex attributes
/// </summary>
void OpenGLMesh::Bind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO));

	bindVertexAttributes();
}

/// <summary>
/// Unbinds the meshes VBO and EBO
/// </summary>
void OpenGLMesh::Unbind() const
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

/// <summary>
/// Sets what shader vertex attributes the mesh will use when rendering
/// </summary>
/// <param name="shaderPos"></param>
void OpenGLMesh::setVertexAttributes(bool shaderPos, bool shaderNorm, bool shaderTex, bool shaderTanBi)
{
	m_vertexAttributesEnabled[e_ShaderPos] = shaderPos;
	m_vertexAttributesEnabled[e_ShaderNorm] = shaderNorm;
	m_vertexAttributesEnabled[e_ShaderTex] = shaderTex;
	m_vertexAttributesEnabled[e_ShaderTanBi] = shaderTanBi;
}

/// <summary>
/// Get the meshes indices, used when calling the glDrawElements() function
/// </summary>
const std::vector<unsigned int>& OpenGLMesh::getIndices() const
{
	return m_meshIndices;
}

/// <summary>
/// Uses the saved filepath (from .setFilePath()) to load the mesh .obj into the object
/// Must be called BEFORE .loadMesh()
/// </summary>
/// <returns>Returns false if .obj can't be found at filepath</returns>
bool OpenGLMesh::readMeshFromFile()
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(m_filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		PRINT_WARN("MESH-> {0} failed to load, loading default mesh", m_filePath);
		return false;
	}

	aiMesh* mesh = scene->mMeshes[0];
	m_meshVertices.reserve(mesh->mNumVertices); // Reserve enough space to hold all the vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// Texcoords
		glm::vec2 vec;
		vec.x = mesh->mTextureCoords[0][i].x;
		vec.y = mesh->mTextureCoords[0][i].y;
		vertex.TexCoords = vec;

		// Tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		// Bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;

		m_meshVertices.push_back(vertex);
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m_meshIndices.push_back(face.mIndices[j]);
		}
	}
	return true;
}

/// <summary>
/// Creates the OpenGL VBO and EBO from the saved mesh data
/// Must be called AFTER .readMeshFromFile()
/// </summary>
void OpenGLMesh::loadMesh()
{
	glCall(glGenBuffers(1, &m_meshVBO));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO));
	glCall(glBufferData(GL_ARRAY_BUFFER, m_meshVertices.size() * sizeof(Vertex), &m_meshVertices[0], GL_STATIC_DRAW));

	glCall(glGenBuffers(1, &m_meshEBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshIndices.size() * sizeof(unsigned int), &m_meshIndices[0], GL_STATIC_DRAW));
}

/// <summary>
/// Sets the texture filepath. Used by .readMeshFromFile()
/// </summary>
void OpenGLMesh::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& OpenGLMesh::getFilePath() const
{
	return m_filePath;
}

/// <summary>
/// Binds the shader vertex attributes which have been previously set by setVertexAttributes()
/// </summary>
void OpenGLMesh::bindVertexAttributes() const
{
	if (m_vertexAttributesEnabled[e_ShaderPos]) // Position
	{
		glCall(glEnableVertexAttribArray(0));
		glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	}

	if (m_vertexAttributesEnabled[e_ShaderNorm]) // Normal
	{
		glCall(glEnableVertexAttribArray(1));
		glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	}

	if (m_vertexAttributesEnabled[e_ShaderTex]) // Texture Coordinates
	{
		glCall(glEnableVertexAttribArray(2));
		glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
	}

	if (m_vertexAttributesEnabled[e_ShaderTanBi]) // Tangents & Bitangents
	{
		glCall(glEnableVertexAttribArray(3));
		glCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));

		glCall(glEnableVertexAttribArray(4));
		glCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)));
	}
}

/// <summary>
/// Creates a mesh object, using the specified filepath.
/// If an object already exists with the same filepath, then return that instead of recreating the same object 
/// </summary>
/// <returns>Pointer to the created mesh object</returns>
std::shared_ptr<OpenGLMesh> MeshManager::retrieveMeshObject(const std::string& filePath)
{
	// Check if model is already loaded loaded
	for (const auto& m : m_loadedMeshes)
	{
		if (m->getFilePath() == filePath)
		{
			return m;
		}
	}

	// Otherwise, create new model and add it to vector
	auto newMesh = std::make_shared<OpenGLMesh>();
	newMesh->setFilePath(filePath);
	m_loadedMeshes.push_back(newMesh);

	return newMesh;
}

/// <summary>
/// Calls the .readMeshFromFile() function for each mesh object created from ::retrieveMeshObject(). This part reads the .obj data from the filepath and puts it into the object.
/// Part 1 of 2 for mesh creation
/// </summary>
void MeshManager::readMeshesFromFile()
{
	for (auto& m : m_loadedMeshes)
	{
		if (!m->readMeshFromFile())
		{
			// If mesh fails to be read, then load in the default cube mesh
			m->setFilePath("res/meshes/cube.obj");
			m->readMeshFromFile();
		}
	}
}

/// <summary>
/// Calls the .loadMesh() function for each mesh object. Uses the saved .obj data saved in the object to create the OpenGL VBO and EBO
/// Part 2 of 2 for mesh creation
/// </summary>
void MeshManager::createMeshes()
{
	for (auto& m : m_loadedMeshes)
	{
		m->loadMesh();
	}
}

/// <summary>
/// Completely clears all saved mesh objects
/// </summary>
void MeshManager::clearMeshes()
{
	m_loadedMeshes.clear();
}
