#include "models/Model.h"

Model::Model()
	:m_modelMesh(nullptr), m_modelShaderPassOne(nullptr), m_modelShaderPassTwo(nullptr), 
	m_position{ 0.0, 0.0, 0.0 }, m_rotation{ 0.0, 0.0, 0.0 }, m_scale{ 1.0f, 1.0f, 1.0f },
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f },
	m_localLightManager(EngineStatics::getLightManager()), m_localProjectionMatrix(EngineStatics::getProjectionMatrix())
{
}

Model::~Model()
{
	m_modelMesh = nullptr;
	m_modelShaderPassOne = nullptr;
	m_modelShaderPassTwo = nullptr;
}

/// <summary>
/// Loads the specified mesh and creates the VBO and EBO buffers
/// </summary>
/// <param name="meshFilePath">Mesh file path</param>
void Model::setMesh(const std::string& meshFilePath)
{
	m_modelMesh = MeshManager::loadMesh(meshFilePath);
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

void Model::SetXPos(float num) { m_position.x = num; }

void Model::SetYPos(float num) { m_position.y = num; }

void Model::SetZPos(float num) { m_position.z = num; }

float Model::GetXPos() const { return m_position.x; }

float Model::GetYPos() const { return m_position.y; }

float Model::GetZPos() const { return m_position.z; }

void Model::SetXRot(float num) { m_rotation.x = num; }

void Model::SetYRot(float num) { m_rotation.y = num; }

void Model::SetZRot(float num) { m_rotation.z = num; }

float Model::GetXRot() const { return m_rotation.x; }

float Model::GetYRot() const { return m_rotation.y; }

float Model::GetZRot() const { return m_rotation.z; }

void Model::SetXScale(float num) { m_scale.x = num; }

void Model::SetYScale(float num) { m_scale.y = num; }

void Model::SetZScale(float num) { m_scale.z = num; }

float Model::GetXScale() const { return m_scale.x; }

float Model::GetYScale() const { return m_scale.y; }

float Model::GetZScale() const { return m_scale.z; }

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
