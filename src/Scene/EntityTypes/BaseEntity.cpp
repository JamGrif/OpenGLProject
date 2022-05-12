#include "pch.h"
#include "Scene/EntityTypes/BaseEntity.h"

#include <glm\gtc\matrix_transform.hpp>

BaseEntity::BaseEntity(templateBaseEntity object)
	:m_modelMesh(nullptr), m_modelShaderPassOne(nullptr), m_modelShaderPassTwo(nullptr),
	m_entityType(object.objectType), m_position(object.position), m_rotation(object.rotation), m_scale(object.scale),
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f },
	m_transformUpdated(false), m_textureUpdated(false), m_editorName(""),
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
void BaseEntity::setMatrixValues()
{
	// Reset matrix values
	m_mMat = glm::mat4(1.0f);
	m_tMat = glm::mat4(1.0f);
	m_rMat = glm::mat4(1.0f);
	m_sMat = glm::mat4(1.0f);

	// Set meshes matrices
	m_tMat = glm::translate(m_tMat, m_position);
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.x), glm::vec3(1.0, 0.0f, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_sMat = glm::scale(m_sMat, m_scale);

	m_mMat = m_tMat * m_rMat * m_sMat;

	m_vMat = EngineStatics::getCamera()->getViewMatrix(); //---------	
}

/// <summary>
/// Sets whether the entity type is LightingEntity, SkyEntity, TerrainEntity etc...
/// </summary>
void BaseEntity::setEntityType(const std::string& type)
{
	m_entityType = type;
}

std::string BaseEntity::getEntityType() const
{
	return m_entityType;
}

/// <summary>
/// Returns if the transform part of the entity has been updated since the last time it was checked
/// If it has been updated then return true, but set the status back to false
/// </summary>
bool BaseEntity::getTransformUpdated()
{
	if (m_transformUpdated)
	{
		// If transform update is checked then reset it back to false after returning it
		bool temp = m_transformUpdated;
		m_transformUpdated = false;
		return temp;
	}
	else
	{
		return m_transformUpdated;
	}
}

bool BaseEntity::getTextureUpdated()
{
	//
}

/// <summary>
/// Sets the name that will be used for this entity in the ImGui editor
/// </summary>
void BaseEntity::setEditorName(const std::string& name)
{
	m_editorName = name;
}

std::string BaseEntity::getEditorName() const
{
	return m_editorName;
}

void BaseEntity::SetXPos(float num) { m_position.x = num; m_transformUpdated = true; }

void BaseEntity::SetYPos(float num) { m_position.y = num; m_transformUpdated = true; }

void BaseEntity::SetZPos(float num) { m_position.z = num; m_transformUpdated = true; }

float BaseEntity::GetXPos() const { return m_position.x; }

float BaseEntity::GetYPos() const { return m_position.y; }

float BaseEntity::GetZPos() const { return m_position.z; }

void BaseEntity::SetXRot(float num) { m_rotation.x = num; m_transformUpdated = true; }

void BaseEntity::SetYRot(float num) { m_rotation.y = num; m_transformUpdated = true; }

void BaseEntity::SetZRot(float num) { m_rotation.z = num; m_transformUpdated = true; }

float BaseEntity::GetXRot() const { return m_rotation.x; }

float BaseEntity::GetYRot() const { return m_rotation.y; }

float BaseEntity::GetZRot() const { return m_rotation.z; }

void BaseEntity::SetXScale(float num) { m_scale.x = num; m_transformUpdated = true;}

void BaseEntity::SetYScale(float num) { m_scale.y = num; m_transformUpdated = true;}

void BaseEntity::SetZScale(float num) { m_scale.z = num; m_transformUpdated = true;}

float BaseEntity::GetXScale() const { return m_scale.x; }

float BaseEntity::GetYScale() const { return m_scale.y; }

float BaseEntity::GetZScale() const { return m_scale.z; }

void BaseEntity::IncXPos(float num) { m_position.x += num; m_transformUpdated = true;}

void BaseEntity::IncYPos(float num) { m_position.y += num; m_transformUpdated = true;}

void BaseEntity::IncZPos(float num) { m_position.z += num; m_transformUpdated = true;}

void BaseEntity::DecXPos(float num) { m_position.x -= num; m_transformUpdated = true;}

void BaseEntity::DecYPos(float num) { m_position.y -= num; m_transformUpdated = true;}

void BaseEntity::DecZPos(float num) { m_position.z -= num; m_transformUpdated = true;}

void BaseEntity::IncXRot(float num) { m_rotation.x += num; m_transformUpdated = true;}

void BaseEntity::IncYRot(float num) { m_rotation.y += num; m_transformUpdated = true;}

void BaseEntity::IncZRot(float num) { m_rotation.z += num; m_transformUpdated = true;}

void BaseEntity::DecXRot(float num) { m_rotation.x -= num; m_transformUpdated = true;}

void BaseEntity::DecYRot(float num) { m_rotation.y -= num; m_transformUpdated = true;}

void BaseEntity::DecZRot(float num) { m_rotation.z -= num; m_transformUpdated = true;}

void BaseEntity::IncXScale(float num) { m_scale.x += num; m_transformUpdated = true;}

void BaseEntity::IncYScale(float num) { m_scale.y += num; m_transformUpdated = true;}

void BaseEntity::IncZScale(float num) { m_scale.z += num; m_transformUpdated = true;}

void BaseEntity::DecXScale(float num) { m_scale.x -= num; m_transformUpdated = true;}

void BaseEntity::DecYScale(float num) { m_scale.y -= num; m_transformUpdated = true;}

void BaseEntity::DecZScale(float num) { m_scale.z -= num; m_transformUpdated = true;}
