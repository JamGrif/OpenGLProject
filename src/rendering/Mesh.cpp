#include "rendering/Mesh.h"

#include <iostream>

#include <assimp/Importer.hpp>		// Mesh loading
#include <assimp/scene.h>			//
#include <assimp/postprocess.h>		//

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

std::vector<std::shared_ptr<Mesh>> MeshManager::m_loadedMeshes;

Mesh::Mesh()
	:m_meshVBO(0), m_meshEBO(0), m_filePath("")
{
}

Mesh::~Mesh()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &m_meshVBO);
	glDeleteBuffers(1, &m_meshEBO);
}


/// <summary>
/// Binds the meshes VBO and EBO and sets the shader vertex attributes
/// </summary>
/// <param name="shaderPos">Use position vertices?</param>
/// <param name="shaderNorm">Use normal vertices?</param>
/// <param name="shaderTex">Use texture coordinates vertices?</param>
/// <param name="shaderTan">Use tangents and bitangents vertices?</param>
void Mesh::Bind(bool shaderPos, bool shaderNorm, bool shaderTex, bool shaderTanBi) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO);

	if (shaderPos) // Position
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	if (shaderNorm) // Normal
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	}

	if (shaderTex) // Texture Coordinates
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	}

	if (shaderTanBi) // Tangents & Bitangents
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}
}

/// <summary>
/// Unbinds the meshes VBO and EBO
/// </summary>
void Mesh::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/// <summary>
/// Get the meshes indices, used when calling the glDrawElements() function
/// </summary>
const std::vector<unsigned int>& Mesh::getIndices() const
{
	return m_meshIndices;
}

/// <summary>
/// Uses the saved filepath (from .setFilePath()) to load the mesh .obj into the object
/// Must be called BEFORE .loadMesh()
/// </summary>
/// <returns>Returns false if .obj can't be found at filepath</returns>
bool Mesh::readMeshFromFile()
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(m_filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

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
void Mesh::loadMesh()
{
	glGenBuffers(1, &m_meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO);
	glBufferData(GL_ARRAY_BUFFER, m_meshVertices.size() * sizeof(Vertex), &m_meshVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_meshEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_meshIndices.size() * sizeof(unsigned int), &m_meshIndices[0], GL_STATIC_DRAW);
}

/// <summary>
/// Sets the texture filepath. Used by .readMeshFromFile()
/// </summary>
void Mesh::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& Mesh::getFilePath() const
{
	return m_filePath;
}

/// <summary>
/// Creates a mesh object, using the specified filepath.
/// If an object already exists with the same filepath, then return that instead of recreating the same object 
/// </summary>
/// <returns>Pointer to the created mesh object</returns>
std::shared_ptr<Mesh> MeshManager::retrieveMeshObject(const std::string& filePath)
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
	auto newMesh = std::make_shared<Mesh>();
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
