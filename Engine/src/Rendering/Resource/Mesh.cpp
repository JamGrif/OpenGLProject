#include "pch.h"
#include "Rendering/Resource/Mesh.h"

#include "assimp/Importer.hpp"		// Importer interface
#include "assimp/scene.h"			// Output data structure
#include "assimp/postprocess.h"		// Post-processing flags

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "GL/glew.h"

static constexpr int NO_BUFFER = 0;

static const std::string MESH_FILEPATH_PREFIX = "res/meshes/";
static const std::string MESH_FILEPATH_SUFFIX = ".obj";

// Numbers correspond to vertex attribute positions in shader
enum class VertexAttribute
{
	POSITION	= 0,
	NORMAL		= 1,
	TEXTURE		= 2,
	TANGENT		= 3,
	BITANGENT	= 4
};

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

Mesh::Mesh()
	:IResource(), m_meshEBO(NO_BUFFER)
{
}

Mesh::~Mesh()
{
	// Unbind and delete buffers
	glCall(glBindBuffer(GL_ARRAY_BUFFER, NO_BUFFER));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NO_BUFFER));

	glCall(glDeleteBuffers(1, &m_OpenGLResourceID));
	glCall(glDeleteBuffers(1, &m_meshEBO));
}

/// <summary>
/// 1 / 2 of mesh creation
/// Parse the .obj file at filepath
/// </summary>
void Mesh::Parse(const std::string& filepath)
{
	std::string meshFilepath = MESH_FILEPATH_PREFIX + filepath + MESH_FILEPATH_SUFFIX;

	Assimp::Importer assimpImporter;
	const aiScene* assimpScene = assimpImporter.ReadFile(meshFilepath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
	{
		PRINT_WARN("MESH-> {0} failed to load", meshFilepath);
		return;
	}

	const aiMesh* mesh = assimpScene->mMeshes[0];

	// Reserve enough space to hold all the vertices
	m_meshVertices.reserve(mesh->mNumVertices);
	
	// Parse vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
		vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;

		vertex.Tangent.x = mesh->mTangents[i].x;
		vertex.Tangent.y = mesh->mTangents[i].y;
		vertex.Tangent.z = mesh->mTangents[i].z;

		vertex.Bitangent.x = mesh->mBitangents[i].x;
		vertex.Bitangent.y = mesh->mBitangents[i].y;
		vertex.Bitangent.z = mesh->mBitangents[i].z;

		m_meshVertices.push_back(vertex);
	}

	// Parse indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m_meshIndices.push_back(face.mIndices[j]);
		}
	}

	m_resourceFilepath = meshFilepath;
}

/// <summary>
/// 2 / 2 of mesh creation
/// Use parsed mesh data to create OpenGL VBO and EBO buffers
/// </summary>
void Mesh::Create()
{
	// Create VBO
	glCall(glGenBuffers(1, &m_OpenGLResourceID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLResourceID));
	glCall(glBufferData(GL_ARRAY_BUFFER, m_meshVertices.size() * sizeof(Vertex), &m_meshVertices[0], GL_STATIC_DRAW));

	glCall(glBindBuffer(GL_ARRAY_BUFFER, NO_BUFFER));

	// Create EBO
	glCall(glGenBuffers(1, &m_meshEBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshIndices.size() * sizeof(unsigned int), &m_meshIndices[0], GL_STATIC_DRAW));
	
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NO_BUFFER));

	m_bIsCreated = true;
}

/// <summary>
/// What the mesh will do when its reset
/// Unused in this class
/// </summary>
void Mesh::Reset()
{
}

/// <summary>
/// Bind the VBO and EBO to the OpenGL context and the vertex attributes to the currently bound shader
/// </summary>
void Mesh::Bind()
{
	// Bind VBO and EBO
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLResourceID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO));

	// Bind vertex attributes
	glCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::POSITION)));
	glCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::POSITION), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	
	glCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::NORMAL)));
	glCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::NORMAL), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	
	glCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TEXTURE)));
	glCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TEXTURE), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
	
	glCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::TANGENT)));
	glCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::TANGENT), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));

	glCall(glEnableVertexAttribArray(static_cast<GLuint>(VertexAttribute::BITANGENT)));
	glCall(glVertexAttribPointer(static_cast<GLuint>(VertexAttribute::BITANGENT), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)));
}

/// <summary>
/// Unbind the VBO and EBO from the OpenGL context
/// </summary>
void Mesh::Unbind()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
