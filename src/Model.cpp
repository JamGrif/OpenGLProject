#include "Model.h"

Model::Model(glm::vec3 position, glm::vec3 rotation)
	:m_modelMesh(nullptr), m_modelShaderPassOne(nullptr), m_modelShaderPassTwo(nullptr), 
	m_VBO(0), m_EBO(0), m_position{ position }, m_rotation{ rotation }, m_scale{ 1.0f,1.0f,1.0f },
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f }
{
	m_localLightManager = EngineStatics::getLightManager();
}

Model::~Model()
{
	if (m_modelMesh != nullptr)
	{
		m_modelMesh = nullptr;
	}

	if (m_modelShaderPassOne != nullptr)
	{
		m_modelShaderPassOne = nullptr;
	}

	if (m_modelShaderPassTwo != nullptr)
	{
		m_modelShaderPassTwo = nullptr;
	}

	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);

}

/// <summary>
/// Loads the specified mesh and creates the VBO and EBO buffers
/// </summary>
/// <param name="meshFilePath">Mesh file path</param>
void Model::setMesh(std::string meshFilePath)
{
	m_modelMesh = MeshManager::loadModel(meshFilePath);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_modelMesh->getVertices().size() * sizeof(Vertex), &m_modelMesh->getVertices()[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_modelMesh->getIndices().size() * sizeof(unsigned int), &m_modelMesh->getIndices()[0], GL_STATIC_DRAW);

}

/// <summary>
/// Sets the models shader used in its first drawing pass
/// </summary>
/// <param name="vertexPath">Vertex shader file path</param>
/// <param name="fragmentPath">Fragment shader file path</param>
void Model::setShaderOne(const char* vertexPath, const char* fragmentPath)
{
	m_modelShaderPassOne = ShaderManager::retrieveShader(vertexPath, fragmentPath);
}

/// <summary>
/// Sets the models shader used in its second drawing pass
/// </summary>
/// <param name="vertexPath">Vertex shader file path</param>
/// <param name="fragmentPath">Fragment shader file path</param>
void Model::setShaderTwo(const char* vertexPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::retrieveShader(vertexPath, fragmentPath);
}

/// <summary>
/// Resets and sets the matrix values of the model
/// </summary>
void Model::setMatrixValues()
{
	//Reset matrix values
	m_mMat = glm::mat4(1.0f);
	m_tMat = glm::mat4(1.0f);
	m_rMat = glm::mat4(1.0f);
	m_sMat = glm::mat4(1.0f);

	//Set meshes matrices
	m_tMat = glm::translate(m_tMat, m_position);
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.x), glm::vec3(1.0, 0.0f, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_sMat = glm::scale(m_sMat, m_scale);

	m_mMat = m_tMat * m_rMat * m_sMat;
	m_vMat = EngineStatics::getCamera()->getViewMatrix();

}

/// <summary>
/// Sets the layout vertex attributes of the models shader
/// </summary>
/// <param name="shaderPos">Does the shader use position vertices?</param>
/// <param name="shaderNorm">Does the shader use normal vertices?</param>
/// <param name="shaderTex">Does the shader use texture coordinates vertices?</param>
/// <param name="shaderTan">Does the shader use tangents vertices?</param>
/// <param name="shaderBiTan">Does the shader use bitangents vertices?</param>
void Model::setVBOAttrib(bool shaderPos, bool shaderNorm, bool shaderTex, bool shaderTan, bool shaderBiTan)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	if (shaderPos)
	{
		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	if (shaderNorm)
	{
		//Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	}

	if (shaderTex)
	{
		//Texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	}

	if (shaderTan)
	{
		//Tangents
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	}

	if (shaderBiTan)
	{
		//Bitangents
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}
}

void Model::SetXPos(float num) { m_position.x = num; }

void Model::SetYPos(float num) { m_position.y = num; }

void Model::SetZPos(float num) { m_position.z = num; }

float Model::GetXPos() { return m_position.x; }

float Model::GetYPos() { return m_position.y; }

float Model::GetZPos() { return m_position.z; }

void Model::SetXRot(float num) { m_rotation.x = num; }

void Model::SetYRot(float num) { m_rotation.y = num; }

void Model::SetZRot(float num) { m_rotation.z = num; }

float Model::GetXRot() { return m_rotation.x; }

float Model::GetYRot() { return m_rotation.y; }

float Model::GetZRot() { return m_rotation.z; }

void Model::SetXScale(float num) { m_scale.x = num; }

void Model::SetYScale(float num) { m_scale.y = num; }

void Model::SetZScale(float num) { m_scale.z = num; }

float Model::GetXScale() { return m_scale.x; }

float Model::GetYScale() { return m_scale.y; }

float Model::GetZScale() { return m_scale.z; }

void Model::IncXPos(float num) { m_position.x += num; }

void Model::IncYPos(float num) { m_position.y += num; }

void Model::IncZPos(float num) { m_position.z += num; }

void Model::DecXPos(float num) { m_position.x -= num; }

void Model::DecYPos(float num) { m_position.y -= num; }

void Model::DecZPos(float num) { m_position.z -= num; }

void Model::IncXRot(float num) { m_rotation.x += num; }

void Model::IncYRot(float num) { m_rotation.y += num; }

void Model::IncZRot(float num) { m_rotation.z += num; }

void Model::DecXRot(float num) { m_rotation.x -= num; }

void Model::DecYRot(float num) { m_rotation.y -= num; }

void Model::DecZRot(float num) { m_rotation.z -= num; }

void Model::IncXScale(float num) { m_scale.x += num; }

void Model::IncYScale(float num) { m_scale.y += num; }

void Model::IncZScale(float num) { m_scale.z += num; }

void Model::DecXScale(float num) { m_scale.x -= num; }

void Model::DecYScale(float num) { m_scale.y -= num; }

void Model::DecZScale(float num) { m_scale.z -= num; }





