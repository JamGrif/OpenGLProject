#include "pch.h"
#include "Scene/EntityTypes/SkyEntity.h"

#include <GL/glew.h>

SkyEntity::SkyEntity(templateSkyEntity object)
	:BaseEntity(object)
{
	setSkyboxTexture(object.skyboxTexture);

	//setShaderOne--------
	setShaderTwo("res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");

	//Skybox uses its own VBO and attribute system to allow the use of a custom cube
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);
}

SkyEntity::~SkyEntity()
{
	m_skyTexture = nullptr;
	glDeleteBuffers(1, &m_skyboxVBO);
}

void SkyEntity::initEntity()
{
	m_skyTexture->setTextureSlot(0);
}

void SkyEntity::updateEntity()
{
}

/// <summary>
/// Overridden method from BaseEntity class - Unused in this class
/// </summary>
void SkyEntity::drawPassOne()
{
	////If no valid model or shader attached
	//if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	//{
	//	return;
	//}
}

/// <summary>
/// Overridden method from BaseEntity class - Used to draw the Skybox
/// </summary>
void SkyEntity::drawPassTwo()
{
	// If no valid model or shader attached
	if (!m_modelShaderPassTwo)
		return;
	
	// Sky cubemap has not loaded correctly, so don't draw object
	if (!m_skyTexture)
		return;
	
	// Bind shader
	m_modelShaderPassTwo->Bind();

	/*
		Bind Vertex values
	*/

	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", glm::mat4(glm::mat3(EngineStatics::getCamera()->getViewMatrix())));
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *m_localProjectionMatrix);
	m_modelShaderPassTwo->setUniform1i("sky", 0);

	m_skyTexture->Bind();

	/*
		Bind VBOs and vertex attributes	
	*/

	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	/*
		Draw
	*/

	m_localRenderer->drawCubemap(36);

	/*
		Post-draw cleanup
	*/

	//m_modelMesh->Unbind();
	m_modelShaderPassTwo->Unbind();
	m_skyTexture->Unbind();
}

void SkyEntity::setSkyboxTexture(const std::string& texturePath)
{
	m_skyTexture = CubemapManager::retrieveCubeMapObject(texturePath);
}
