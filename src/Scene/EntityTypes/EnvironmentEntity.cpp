#include "pch.h"
#include "Scene/EntityTypes/EnvironmentEntity.h"

EnvironmentEntity::EnvironmentEntity()
	:m_skyTexture(nullptr), m_usingReflection(false), m_usingRefraction(false)
{
	//setShaderOne--------
	setShaderTwo("res/shaders/environmentMapping-vertex.glsl", "res/shaders/environmentMapping-fragment.glsl");

	// Get the sky cubemap
	m_skyTexture = CubemapManager::retrieveCubeMapObject();
}

EnvironmentEntity::~EnvironmentEntity()
{
	m_skyTexture = nullptr;
}

void EnvironmentEntity::initEntity()
{
	m_modelMesh->setVertexAttributes(true, true, false, false);
	m_skyTexture->setTextureSlot(0);
}

void EnvironmentEntity::updateEntity()
{

}

/// <summary>
/// Overridden method from Model base class - Unused in this class
/// </summary>
void EnvironmentEntity::drawPassOne()
{
	////If no valid model or shader attached
	//if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	//{
	//	return;
	//}
}

/// <summary>
/// Overridden method from Model base class - Used for environment mapping by taking the Skybox texture and mapping it to the texture
/// </summary>
void EnvironmentEntity::drawPassTwo()
{
	// If no valid model or shader attached
	if (!m_modelMesh|| !m_modelShaderPassTwo)
		return;
	
	// If not using either environment mapping types, stop drawing object
	if (!m_usingReflection && !m_usingRefraction)
		return;
	
	// Sky cubemap has not loaded correctly, so don't draw object
	if (!m_skyTexture)
		return;
	
	// Bind shader
	m_modelShaderPassTwo->Bind();

	/*
		Set Vertex values
	*/
	
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *m_localProjectionMatrix);

	/*
		Set Fragment values
	*/

	m_modelShaderPassTwo->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());
	m_modelShaderPassTwo->setUniform1i("sky", 0);
	m_modelShaderPassTwo->setUniform1i("usingReflection", m_usingReflection);
	m_modelShaderPassTwo->setUniform1i("usingRefraction", m_usingRefraction);

	m_skyTexture->Bind();

	/*
		Bind VBOs and vertex attributes
	*/

	m_modelMesh->Bind();

	/*
		Draw
	*/

	m_localRenderer->draw(m_modelMesh->getIndices().size());

	/*
		Post-draw cleanup
	*/
	m_modelMesh->Unbind();
	m_modelShaderPassTwo->Unbind();
	m_skyTexture->Unbind();
}

void EnvironmentEntity::toggleReflection(bool value)
{
	m_usingReflection = value;
}

void EnvironmentEntity::toggleRefraction(bool value)
{
	m_usingRefraction = value;
}
