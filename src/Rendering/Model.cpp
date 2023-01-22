#include "pch.h"
#include "Rendering/Model.h"

#include "Scene/SceneLightManager.h"

#include <glm\gtc\matrix_transform.hpp>

#include "Rendering/MaterialManager.h"
#include "Rendering/MeshManager.h"
#include "Rendering/OpenGLRenderer.h"
#include "Scene/SceneCamera.h"

Model::Model(const ModelLoaderParams& pParams)
	:m_materialID(pParams.materialID), m_meshID(pParams.meshID),
	m_position(pParams.posX, pParams.posY, pParams.posZ),
	m_rotation(pParams.rotX, pParams.rotY, pParams.rotZ),
	m_scale(pParams.scaleX, pParams.scaleY, pParams.scaleZ),
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f },
	m_pSceneCamera(nullptr), m_programProjectionMatrix(TheOpenGLRenderer::Instance()->GetProjectionMatrix())
{
	// Use the meshID to create initial mesh
	MeshManager::Instance()->AddMesh(m_meshID);
}

Model::~Model()
{
}

/// <summary>
/// Perform update operations for model
/// </summary>
void Model::UpdateModel()
{
	SetMatrixValues();
}

/// <summary>
/// Perform drawing operations for model
/// </summary>
void Model::DrawModel()
{
	// Bind material and mesh
	TheMaterialManager::Instance()->BindMaterialAtID(m_materialID, m_mMat);
	TheMeshManager::Instance()->BindMeshAtID(m_meshID);
	
	// Draw
	TheOpenGLRenderer::Instance()->Draw(TheMeshManager::Instance()->GetIndicesCountAtID(m_meshID));

	// Unbind material and mesh
	TheMeshManager::Instance()->UnbindMeshAtID(m_meshID);
	TheMaterialManager::Instance()->UnbindMaterialAtID(m_materialID);
}

/// <summary>
/// Set the pointers the model uses from the scene
/// </summary>
void Model::SetModelPointers(SceneCamera* pSceneCamera)
{
	m_pSceneCamera = pSceneCamera;
}

/// <summary>
/// Resets and sets the matrix values of the model
/// </summary>
void Model::SetMatrixValues()
{
	// Reset rotation matrix
	m_rMat = glm::mat4(1.0f);

	// Set transform matrices
	m_tMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.GetX(), m_position.GetY(), m_position.GetZ()));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.GetX()), glm::vec3(1.0, 0.0f, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.GetY()), glm::vec3(0.0f, 1.0, 0.0f));
	m_rMat = glm::rotate(m_rMat, glm::radians(m_rotation.GetZ()), glm::vec3(0.0f, 0.0f, 1.0f));
	m_sMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale.GetX(), m_scale.GetY(), m_scale.GetZ()));

	m_mMat = m_tMat * m_rMat * m_sMat;

	// Set new view matrix
	m_vMat = m_pSceneCamera->GetViewMatrix();
}

