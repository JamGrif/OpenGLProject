#include "pch.h"
#include "Scene/EntityTypes/BasicEntity.h"

#include "Scene/SceneLightManager.h"

BasicEntity::BasicEntity(templateBasicEntity object)
	:BaseEntity(object), m_defaultColour(1.0, 1.0, 1.0), m_copyingLight(false), m_localPointLight(nullptr)
{
	setMesh(object.mesh);

	copyPointLight(object.lightToCopy);

	setShaderTwo("res/shaders/basic-vertex.glsl", "res/shaders/basic-fragment.glsl");
}

BasicEntity::~BasicEntity()
{
	m_localPointLight = nullptr;
}

void BasicEntity::initEntity()
{
	m_modelMesh->setVertexAttributes(true, false, false, false);
}

void BasicEntity::updateEntity()
{
}

/// <summary>
/// Overridden method from BaseEntity class - Unused in this class
/// </summary>
void BasicEntity::drawPassOne()
{
	//// If no valid model or shader attached
	//if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	//{
	//	return;
	//}
}

/// <summary>
/// Overridden method from BaseEntity class - Used to draw a simple object with no lighting applied. Has the option to "copy" the position and colour of a point light
/// </summary>
void BasicEntity::drawPassTwo()
{
	// If no valid model or shader attached
	if (!m_modelMesh || !m_modelShaderPassTwo)
		return;
	

	// If copying light, ensure the light is active otherwise leave draw function
	if (m_copyingLight)
		if (!m_localPointLight->lightActive) //If light is not active, leave draw function
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

	if (m_copyingLight) //Check if copying light
	{
		m_position = m_localPointLight->Position;
		m_modelShaderPassTwo->setUniform3f("blockColour", m_localPointLight->Diffuse);
	}
	else
	{
		m_modelShaderPassTwo->setUniform3f("blockColour", m_defaultColour);
	}

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
}

/// <summary>
/// Indicates that this model will copy the values of a created scene pointlight (specifically its position and colour)
/// </summary>
/// <param name="index">The index of the point light in the point light vector that will be copied</param>
void BasicEntity::copyPointLight(unsigned int index)
{
	if (!m_localLightManager->getPointLight(index)) // If pointlight at specified index does not exist, ignore and return
		return;
	
	unsigned int totalCurrentPointLights = m_localLightManager->getCurrentPointLights();
	if (index <= totalCurrentPointLights && totalCurrentPointLights != 0) // Ensure index is within the range of created point lights
	{
		m_copyingLight = true;
		m_localPointLight = m_localLightManager->getPointLight(index);
	}		
}
