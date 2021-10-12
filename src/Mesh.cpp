#include "Mesh.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<Mesh*> MeshManager::m_loadedModels;

Mesh::Mesh()
	:m_filePath("")
{
}

Mesh::~Mesh()
{
}

bool Mesh::loadMesh(const char* filePath)
{
	m_filePath = filePath;
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

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

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		//Position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		//Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		//Texcoords
		glm::vec2 vec;
		vec.x = mesh->mTextureCoords[0][i].x;
		vec.y = mesh->mTextureCoords[0][i].y;
		vertex.TexCoords = vec;

		//Tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		//Bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;

		m_vertices.push_back(vertex);
	}

	//Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m_indices.push_back(face.mIndices[j]);
		}
	}
	return true;
}


std::vector<Vertex> Mesh::getVertices() const
{
	return m_vertices;
}

std::vector<unsigned int> Mesh::getIndices() const
{
	return m_indices;
}

const char* Mesh::getFilePath() const
{
	return m_filePath;
}

/// <summary>
/// Loads the specified mesh, if mesh already exists it returns a pointer to it instead of reloading the same mesh
/// </summary>
/// <param name="filePath">Mesh file path</param>
/// <returns>Pointer to the loaded mesh</returns>
Mesh* MeshManager::loadModel(const char* filePath)
{
	//Check if model is already loaded loaded
	for (Mesh* im : m_loadedModels)
	{
		if (im->getFilePath() == filePath)
		{
			return im;
		}
	}

	//Otherwise, create new model and add it to vector

	Mesh* newMesh = new Mesh;

	if (!newMesh->loadMesh(filePath)) //Attempt to load texture
	{
		//Texture failed to load so check if missing texture texture is already loaded and then return it
		for (Mesh* m : m_loadedModels)
		{
			if (m->getFilePath() == "res/meshes/cube.obj")
			{
				delete newMesh;
				newMesh = nullptr;

				return m;
			}
		}

		//The missing texture texture has not already been made so make it
		newMesh->loadMesh("res/meshes/cube.obj");
	}

	m_loadedModels.push_back(newMesh);
	return m_loadedModels.back();
}

void MeshManager::clearMeshes()
{
	for (Mesh* m : m_loadedModels)
	{
		delete m;
		m = nullptr;
	}
	m_loadedModels.clear();
}
