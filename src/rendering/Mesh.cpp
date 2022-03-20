#include "rendering/Mesh.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<std::shared_ptr<Mesh>> MeshManager::m_loadedMeshes;

Mesh::Mesh()
	:m_filePath("")
{
}

Mesh::~Mesh()
{
	////glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

bool Mesh::loadMesh(const std::string& filePath)
{
	m_filePath = filePath;
	Assimp::Importer import;
	const aiScene* const scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "MESH->" << m_filePath << " failed to load, loading default mesh - FAILURE" << std::endl;
		return false;
	}
	else
	{
		std::cout << "MESH->" << m_filePath << " successfully loaded - SUCCESS" << std::endl;
	}

	aiMesh* mesh = scene->mMeshes[0];

	m_vertices.reserve(mesh->mNumVertices); // Reserve enough space to hold all the vertices
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

		m_vertices.push_back(vertex);
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m_indices.push_back(face.mIndices[j]);
		}
	}


	setupMesh();

	return true;
}

/// <summary>
/// Creates the OpenGL VBO and EBO
/// </summary>
void Mesh::setupMesh()
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
}


/// <summary>
/// Sets the layout vertex attributes of the models shader
/// </summary>
/// <param name="shaderPos">Does the shader use position vertices?</param>
/// <param name="shaderNorm">Does the shader use normal vertices?</param>
/// <param name="shaderTex">Does the shader use texture coordinates vertices?</param>
/// <param name="shaderTan">Does the shader use tangents vertices?</param>
/// <param name="shaderBiTan">Does the shader use bitangents vertices?</param>
void Mesh::setVBOAttrib(bool shaderPos, bool shaderNorm, bool shaderTex, bool shaderTan, bool shaderBiTan)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	if (shaderPos)
	{
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	if (shaderNorm)
	{
		// Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	}

	if (shaderTex)
	{
		// Texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	}

	if (shaderTan)
	{
		// Tangents
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	}

	if (shaderBiTan)
	{
		// Bitangents
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}
}

const std::vector<unsigned int>& Mesh::getIndices() const
{
	return m_indices;
}

const std::string& Mesh::getFilePath() const
{
	return m_filePath;
}

// MeshManager --------------------------------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// Loads the specified mesh, if mesh already exists it returns a pointer to it instead of reloading the same mesh
/// </summary>
/// <param name="filePath">Mesh file path</param>
/// <returns>Pointer to the loaded mesh</returns>
std::shared_ptr<Mesh> MeshManager::loadMesh(const std::string& filePath)
{
	// Check if model is already loaded loaded
	for (std::shared_ptr<Mesh> im : m_loadedMeshes)
	{
		if (im->getFilePath() == filePath)
		{
			return im;
		}
	}

	// Otherwise, create new model and add it to vector
	std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>();

	if (!newMesh->loadMesh(filePath)) //Attempt to load texture
	{
		// Texture failed to load so check if missing texture texture is already loaded and then return it
		for (std::shared_ptr<Mesh> m : m_loadedMeshes)
		{
			if (m->getFilePath() == "res/meshes/cube.obj")
			{
				return m;
			}
		}

		// The missing texture texture has not already been made so make it
		newMesh->loadMesh("res/meshes/cube.obj");
	}

	m_loadedMeshes.push_back(newMesh);
	return newMesh;
}

void MeshManager::clearMeshes()
{
	m_loadedMeshes.clear();
}
