#include "pch.h"
#include "Scene/EntityTypes/BaseEntity.h"

BaseEntity::BaseEntity()
	:m_modelMesh(nullptr), m_modelShaderPassOne(nullptr), m_modelShaderPassTwo(nullptr), 
	m_position{ 0.0, 0.0, 0.0 }, m_rotation{ 0.0, 0.0, 0.0 }, m_scale{ 1.0f, 1.0f, 1.0f },
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f },
	m_localLightManager(EngineStatics::getLightManager()), m_localRenderer(EngineStatics::getRenderer()), m_localProjectionMatrix(EngineStatics::getProjectionMatrix())
{
}

BaseEntity::~BaseEntity()
{
	m_modelMesh = nullptr;
	m_modelShaderPassOne = nullptr;
	m_modelShaderPassTwo = nullptr;
}

/// <summary>
/// Loads the specified mesh and creates the VBO and EBO buffers
/// </summary>
/// <param name="meshFilePath">Mesh file path</param>
void BaseEntity::setMesh(const std::string& meshFilePath)
{
	m_modelMesh = MeshManager::retrieveMeshObject(meshFilePath);
}

/// <summary>
/// Sets the models shader used in its first drawing pass
/// </summary>
/// <param name="vertexPath">Vertex shader file path</param>
/// <param name="fragmentPath">Fragment shader file path</param>
void BaseEntity::setShaderOne(const char* vertexPath, const char* fragmentPath)
{
	m_modelShaderPassOne = ShaderManager::retrieveShader(vertexPath, fragmentPath);
}

/// <summary>
/// Sets the models shader used in its second drawing pass
/// </summary>
/// <param name="vertexPath">Vertex shader file path</param>
/// <param name="fragmentPath">Fragment shader file path</param>
void BaseEntity::setShaderTwo(const char* vertexPath, const char* fragmentPath)
{
	m_modelShaderPassTwo = ShaderManager::retrieveShader(vertexPath, fragmentPath);
}

/// <summary>
/// Resets and sets the matrix values of the model
/// </summary>
void BaseEntity::setMatrixValues(bool includeVmat)
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

	if (includeVmat)
	{
		m_vMat = EngineStatics::getCamera()->getViewMatrix(); //---------
	}
}

void BaseEntity::SetXPos(float num) { m_position.x = num; }

void BaseEntity::SetYPos(float num) { m_position.y = num; }

void BaseEntity::SetZPos(float num) { m_position.z = num; }

float BaseEntity::GetXPos() const { return m_position.x; }

float BaseEntity::GetYPos() const { return m_position.y; }

float BaseEntity::GetZPos() const { return m_position.z; }

void BaseEntity::SetXRot(float num) { m_rotation.x = num; }

void BaseEntity::SetYRot(float num) { m_rotation.y = num; }

void BaseEntity::SetZRot(float num) { m_rotation.z = num; }

float BaseEntity::GetXRot() const { return m_rotation.x; }

float BaseEntity::GetYRot() const { return m_rotation.y; }

float BaseEntity::GetZRot() const { return m_rotation.z; }

void BaseEntity::SetXScale(float num) { m_scale.x = num; }

void BaseEntity::SetYScale(float num) { m_scale.y = num; }

void BaseEntity::SetZScale(float num) { m_scale.z = num; }

float BaseEntity::GetXScale() const { return m_scale.x; }

float BaseEntity::GetYScale() const { return m_scale.y; }

float BaseEntity::GetZScale() const { return m_scale.z; }

void BaseEntity::IncXPos(float num) { m_position.x += num; }

void BaseEntity::IncYPos(float num) { m_position.y += num; }

void BaseEntity::IncZPos(float num) { m_position.z += num; }

void BaseEntity::DecXPos(float num) { m_position.x -= num; }

void BaseEntity::DecYPos(float num) { m_position.y -= num; }

void BaseEntity::DecZPos(float num) { m_position.z -= num; }

void BaseEntity::IncXRot(float num) { m_rotation.x += num; }

void BaseEntity::IncYRot(float num) { m_rotation.y += num; }

void BaseEntity::IncZRot(float num) { m_rotation.z += num; }

void BaseEntity::DecXRot(float num) { m_rotation.x -= num; }

void BaseEntity::DecYRot(float num) { m_rotation.y -= num; }

void BaseEntity::DecZRot(float num) { m_rotation.z -= num; }

void BaseEntity::IncXScale(float num) { m_scale.x += num; }

void BaseEntity::IncYScale(float num) { m_scale.y += num; }

void BaseEntity::IncZScale(float num) { m_scale.z += num; }

void BaseEntity::DecXScale(float num) { m_scale.x -= num; }

void BaseEntity::DecYScale(float num) { m_scale.y -= num; }

void BaseEntity::DecZScale(float num) { m_scale.z -= num; }
