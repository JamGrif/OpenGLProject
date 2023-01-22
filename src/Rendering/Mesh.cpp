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
	:m_meshVBO(-1), m_meshEBO(-1), m_bIsCreated(false)
{
}

Mesh::~Mesh()
{
	//PRINT_TRACE("deleting mesh object with filepath of {0}", m_meshFilePath);

	// Unbind current VBO / EBO
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// Delete VBO / EBO
	glCall(glDeleteBuffers(1, &m_meshVBO));
	glCall(glDeleteBuffers(1, &m_meshEBO));
}

void Mesh::ParseMesh(const std::string& filepath)
{
	Assimp::Importer assimpImporter;
	const aiScene* assimpScene = assimpImporter.ReadFile(filepath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
	{
		PRINT_WARN("MESH-> {0} failed to load", filepath);
		return;
	}

	const aiMesh* mesh = assimpScene->mMeshes[0];
	m_meshVertices.reserve(mesh->mNumVertices); // Reserve enough space to hold all the vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Position
		//glm::vec3 vector;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		// Normals
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		// Texcoords
		//glm::vec2 vec;
		vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
		vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;

		// Tangent
		vertex.Tangent.x = mesh->mTangents[i].x;
		vertex.Tangent.y = mesh->mTangents[i].y;
		vertex.Tangent.z = mesh->mTangents[i].z;

		// Bitangent
		vertex.Bitangent.x = mesh->mBitangents[i].x;
		vertex.Bitangent.y = mesh->mBitangents[i].y;
		vertex.Bitangent.z = mesh->mBitangents[i].z;

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

	m_meshFilePath = filepath;

}

void Mesh::CreateMesh()
{
	// Create and setup the meshes VBO
	glCall(glGenBuffers(1, &m_meshVBO));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO));
	glCall(glBufferData(GL_ARRAY_BUFFER, m_meshVertices.size() * sizeof(Vertex), &m_meshVertices[0], GL_STATIC_DRAW));

	// Create and setup the meshes EBO
	glCall(glGenBuffers(1, &m_meshEBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshIndices.size() * sizeof(unsigned int), &m_meshIndices[0], GL_STATIC_DRAW));

	m_bIsCreated = true;
}

void Mesh::BindMesh()
{
	// Bind the meshes VBO and EBO
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO));

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

void Mesh::UnbindMesh()
{
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
