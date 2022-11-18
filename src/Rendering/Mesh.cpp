#include "pch.h"
#include "Rendering/Mesh.h"

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


Mesh::Mesh()
	:m_meshOpenGLVBO(-1), m_meshOpenGLEBO(-1), m_bIsCreated(false)
{
}

Mesh::~Mesh()
{
	//PRINT_TRACE("deleting mesh object with filepath of {0}", m_meshFilePath);

	// Unbind current VBO / EBO
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// Delete VBO / EBO
	glCall(glDeleteBuffers(1, &m_meshOpenGLVBO));
	glCall(glDeleteBuffers(1, &m_meshOpenGLEBO));
}

void Mesh::parseMesh(const std::string& filepath)
{
	//PRINT_TRACE("parsing mesh object with filepath of {0}", filepath);

	m_meshFilePath = filepath;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(m_meshFilePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		PRINT_WARN("MESH-> {0} failed to load", m_meshFilePath);
		return;
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
}

void Mesh::createMesh()
{
	// Create and setup the meshes VBO
	glCall(glGenBuffers(1, &m_meshOpenGLVBO));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_meshOpenGLVBO));
	glCall(glBufferData(GL_ARRAY_BUFFER, m_meshVertices.size() * sizeof(Vertex), &m_meshVertices[0], GL_STATIC_DRAW));

	// Create and setup the meshes EBO
	glCall(glGenBuffers(1, &m_meshOpenGLEBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshOpenGLEBO));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshIndices.size() * sizeof(unsigned int), &m_meshIndices[0], GL_STATIC_DRAW));

	m_bIsCreated = true;
}

void Mesh::bindMesh()
{
	// Bind the meshes VBO and EBO
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_meshOpenGLVBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshOpenGLEBO));

	// Position
	glCall(glEnableVertexAttribArray(0));
	glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	
	 // Normal
	glCall(glEnableVertexAttribArray(1));
	glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	
	// Texture Coordinates
	glCall(glEnableVertexAttribArray(2));
	glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));
	
	 // Tangents & Bitangents
	glCall(glEnableVertexAttribArray(3));
	glCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));

	glCall(glEnableVertexAttribArray(4));
	glCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)));
}

void Mesh::unbindMesh()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
