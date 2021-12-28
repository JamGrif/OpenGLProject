#include "ModelBasic.h"

ModelBasic::ModelBasic()
	:m_defaultColour(1.0, 1.0, 1.0), NOT_COPYING_LIGHT(-1), m_pointLightToCopy(NOT_COPYING_LIGHT)
{

	//setShaderOne--------
	setShaderTwo("res/shaders/basic-vertex.glsl", "res/shaders/basic-fragment.glsl");
}

ModelBasic::~ModelBasic()
{

}

/// <summary>
/// Overridden method from Model base class - Unused in this class
/// </summary>
void ModelBasic::drawPassOne()
{
	// If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

/// <summary>
/// Overridden method from Model base class - Used to draw a simple object with no lighting applied. Has the option to "copy" the position and colour of a point light
/// </summary>
void ModelBasic::drawPassTwo()
{
	// If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}
	
	// Bind shader
	m_modelShaderPassTwo->Bind();

	
	/*
		Set Vertex values
	*/

	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());

	/*
		Set Fragment values
	*/

	if (m_pointLightToCopy != NOT_COPYING_LIGHT) //Check if copying light
	{
		if (!m_localLightManager->getPointLight(m_pointLightToCopy)->lightActive) //If light is not active, leave draw function 
			return;

		m_position = m_localLightManager->getPointLight(m_pointLightToCopy)->Position;
		m_modelShaderPassTwo->setUniform3f("blockColour", m_localLightManager->getPointLight(m_pointLightToCopy)->Diffuse);
	}
	else
	{
		m_modelShaderPassTwo->setUniform3f("blockColour", m_defaultColour);
	}

	/*
		Bind VBOs and vertex attributes
	*/

	setVBOAttrib(true, false, false, false, false);

	/*
		Draw
	*/

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_modelMesh->getIndices().size()), GL_UNSIGNED_INT, 0);

	/*
		Post-draw cleanup
	*/

	m_modelShaderPassTwo->Unbind();
	
}

/// <summary>
/// Indicates that this model will copy the values of a created scene pointlight (specifically its position and colour)
/// </summary>
/// <param name="index">The index of the point light in the point light vector that will be copied</param>
void ModelBasic::copyPointLight(int index)
{
	if (m_localLightManager->getPointLight(index) == nullptr) // If pointlight at specified index does not exist, ignore and return
	{
		return;
	}

	if (index <= m_localLightManager->getCurrentPointLights() && m_localLightManager->getCurrentPointLights() != 0) // Ensure index is within the range of created point lights
	{
		m_pointLightToCopy = index;
	}		
}
