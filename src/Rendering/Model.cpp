#include "pch.h"
#include "Rendering/Model.h"

#include "Scene/SceneLightManager.h"

#include "glm\gtc\matrix_transform.hpp"

#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Rendering/OpenGLRenderer.h"
#include "Scene/SceneCamera.h"

Model::Model(const ModelLoaderParams& pParams)
	:m_modelID(pParams.modelID), m_materialID(pParams.materialID), m_meshID(pParams.meshID),
	m_position(pParams.position), m_rotation(pParams.rotation), m_scale(pParams.scale),
	m_mMat{ 1.0f }, m_vMat{ 1.0f }, m_tMat{ 1.0f }, m_rMat{ 1.0f }, m_sMat{ 1.0f },
	m_programProjectionMatrix(TheOpenGLRenderer::Get()->GetProjectionMatrix())
{
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
	MeshManager::Get()->BindResourceAtID(m_meshID);
	
	// Draw ----
	TheOpenGLRenderer::Get()->Draw(MeshManager::Get()->GetResourceAtID(m_meshID)->GetIndicesCount());

	// Unbind material and mesh
	MeshManager::Get()->UnbindResourceAtID(m_meshID);
	TheMaterialManager::Instance()->UnbindMaterialAtID(m_materialID);
}

/// <summary>
/// Set the pointers the model uses from the scene
/// </summary>
void Model::SetModelPointers(std::weak_ptr<SceneCamera> pSceneCamera)
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
	m_vMat = m_pSceneCamera.lock()->GetViewMatrix();
}

