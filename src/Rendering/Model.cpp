#include "pch.h"
#include "Rendering/Model.h"

#include "Scene/SceneLightManager.h"

#include <glm\gtc\matrix_transform.hpp>

#include "Rendering/MaterialManager.h"
#include "Rendering/MeshManager.h"
#include "Rendering/OpenGLRenderer.h"
#include "Scene/SceneCamera.h"

Model::Model(ModelLoaderParams pParams)
	:m_materialID(pParams.materialID), m_meshID(pParams.meshID),
	m_position(pParams.posX, pParams.posY, pParams.posZ), m_rotation(pParams.rotX, pParams.rotY, pParams.rotZ),
	m_scale(pParams.scaleX, pParams.scaleY, pParams.scaleZ),
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f },
	m_pSceneLightManager(nullptr), m_pSceneCamera(nullptr), m_programProjectionMatrix(TheOpenGLRenderer::Instance()->getProjectionMatrix())
{
	// Use the meshID to create initial mesh
	MeshManager::Instance()->addMesh(m_meshID);
}

Model::~Model()
{
}

void Model::updateModel()
{
	setMatrixValues();
}

void Model::drawModel()
{
	// Bind material and mesh
	TheMaterialManager::Instance()->bindMaterialAtID(m_materialID, m_mMat);
	TheMeshManager::Instance()->bindMeshAtID(m_meshID);
	
	// Draw
	TheOpenGLRenderer::Instance()->draw(TheMeshManager::Instance()->getIndicesCountAtID(m_meshID));

	// Unbind material and mesh
	TheMeshManager::Instance()->unbindMeshAtID(m_meshID);
	TheMaterialManager::Instance()->unbindMaterialAtID(m_materialID);
}


void Model::setModelPointers(SceneCamera* pSceneCamera, SceneLightManager* pSceneLightManager)
{
	m_pSceneCamera = pSceneCamera;
	m_pSceneLightManager = pSceneLightManager;
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
	m_tMat = glm::translate(m_tMat, glm::vec3(m_position.getX(), m_position.getY(), m_position.getZ()));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.getX()), glm::vec3(1.0, 0.0f, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.getY()), glm::vec3(0.0f, 1.0, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));
	m_sMat = glm::scale(m_sMat, glm::vec3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	m_mMat = m_tMat * m_rMat * m_sMat;

	m_vMat = m_pSceneCamera->getViewMatrix();
}

