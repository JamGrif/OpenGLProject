#include "pch.h"
#include "Rendering/Model.h"

#include "Scene/SceneLightManager.h"

#include <glm\gtc\matrix_transform.hpp>

#include "Rendering/MaterialManager.h"
#include "Rendering/MeshManager.h"

Model::Model(ModelLoaderParams pParams)
	:m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f },
	m_transformUpdated(false), m_textureUpdated(false), m_editorName(""),
	m_localLightManager(EngineStatics::getLightManager()), m_localProjectionMatrix(EngineStatics::getProjectionMatrix())
{

	m_entityType = pParams.objectType;
	m_position = pParams.position;
	m_rotation = pParams.rotation;
	m_scale = pParams.scale;

	setMaterialID(pParams.materialID);

	m_meshID = pParams.meshID;

	MeshManager::Instance()->createMesh(m_meshID, pParams.meshFilepath);
	

}

Model::~Model()
{
}

void Model::initEntity()
{
	setMatrixValues();
}

void Model::updateEntity()
{
}

/// <summary>
/// Overridden method from BaseEntity class - Used to render the scene to the shadow map depth texture
/// NOT PROPERLY IMPLEMENTED
/// </summary>
void Model::drawPassOne()
{
}

/// <summary>
/// Overridden method from BaseEntity class - Used to light an object by taking values from all active lights, binding texture maps and drawing the object
/// </summary>
void Model::drawPassTwo()
{
	// Bind material and mesh
	TheMaterialManager::Instance()->getMaterialAtID(m_materialID)->bindMaterial(m_mMat, m_vMat, m_localLightManager, *m_localProjectionMatrix);
	TheMeshManager::Instance()->getMeshAtID(m_meshID)->bindMesh();
	
	// Draw
	TheOpenGLRenderer::Instance()->draw(TheMeshManager::Instance()->getMeshAtID(m_meshID)->getIndices().size());

	// Unbind material and mesh
	TheMeshManager::Instance()->getMeshAtID(m_meshID)->unbindMesh();
	TheMaterialManager::Instance()->getMaterialAtID(m_materialID)->unbindMaterial();
}

/// <summary>
/// Resets and sets the matrix values of the model
/// </summary>
void Model::setMatrixValues()
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
void Model::setEntityType(const std::string& type)
{
	m_entityType = type;
}

std::string Model::getEntityType() const
{
	return m_entityType;
}

/// <summary>
/// Returns if the transform part of the entity has been updated since the last time it was checked
/// If it has been updated then return true, but set the status back to false
/// </summary>
bool Model::getTransformUpdated()
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

/// <summary>
/// Returns if the texture part of the entity has been updated since the last time it was checked
/// </summary>
bool Model::getTextureUpdated()
{
	// TO DO
	return false;
}

/// <summary>
/// Sets the name that will be used for this entity in the ImGui editor
/// </summary>
void Model::setEditorName(const std::string& name)
{
	m_editorName = name;
}

std::string Model::getEditorName() const
{
	return m_editorName;
}

void Model::SetXPos(float num) { m_position.x = num; m_transformUpdated = true; }

void Model::SetYPos(float num) { m_position.y = num; m_transformUpdated = true; }

void Model::SetZPos(float num) { m_position.z = num; m_transformUpdated = true; }

float Model::GetXPos() const { return m_position.x; }

float Model::GetYPos() const { return m_position.y; }

float Model::GetZPos() const { return m_position.z; }

void Model::SetXRot(float num) { m_rotation.x = num; m_transformUpdated = true; }

void Model::SetYRot(float num) { m_rotation.y = num; m_transformUpdated = true; }

void Model::SetZRot(float num) { m_rotation.z = num; m_transformUpdated = true; }

float Model::GetXRot() const { return m_rotation.x; }

float Model::GetYRot() const { return m_rotation.y; }

float Model::GetZRot() const { return m_rotation.z; }

void Model::SetXScale(float num) { m_scale.x = num; m_transformUpdated = true; }

void Model::SetYScale(float num) { m_scale.y = num; m_transformUpdated = true; }

void Model::SetZScale(float num) { m_scale.z = num; m_transformUpdated = true; }

float Model::GetXScale() const { return m_scale.x; }

float Model::GetYScale() const { return m_scale.y; }

float Model::GetZScale() const { return m_scale.z; }

void Model::IncXPos(float num) { m_position.x += num; m_transformUpdated = true; }

void Model::IncYPos(float num) { m_position.y += num; m_transformUpdated = true; }

void Model::IncZPos(float num) { m_position.z += num; m_transformUpdated = true; }

void Model::DecXPos(float num) { m_position.x -= num; m_transformUpdated = true; }

void Model::DecYPos(float num) { m_position.y -= num; m_transformUpdated = true; }

void Model::DecZPos(float num) { m_position.z -= num; m_transformUpdated = true; }

void Model::IncXRot(float num) { m_rotation.x += num; m_transformUpdated = true; }

void Model::IncYRot(float num) { m_rotation.y += num; m_transformUpdated = true; }

void Model::IncZRot(float num) { m_rotation.z += num; m_transformUpdated = true; }

void Model::DecXRot(float num) { m_rotation.x -= num; m_transformUpdated = true; }

void Model::DecYRot(float num) { m_rotation.y -= num; m_transformUpdated = true; }

void Model::DecZRot(float num) { m_rotation.z -= num; m_transformUpdated = true; }

void Model::IncXScale(float num) { m_scale.x += num; m_transformUpdated = true; }

void Model::IncYScale(float num) { m_scale.y += num; m_transformUpdated = true; }

void Model::IncZScale(float num) { m_scale.z += num; m_transformUpdated = true; }

void Model::DecXScale(float num) { m_scale.x -= num; m_transformUpdated = true; }

void Model::DecYScale(float num) { m_scale.y -= num; m_transformUpdated = true; }

void Model::DecZScale(float num) { m_scale.z -= num; m_transformUpdated = true; }
