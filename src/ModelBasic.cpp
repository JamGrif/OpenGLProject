#include "ModelBasic.h"

ModelBasic::ModelBasic(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation), m_defaultColour(1.0, 1.0, 1.0), m_pointLightToCopy(-1)
{
	//m_scale.x = 0.3f;
	//m_scale.y = 0.3f;
	//m_scale.z = 0.3f;

	//setShaderOne--------
	setShaderTwo("res/shaders/basic-vertex.glsl", "res/shaders/basic-fragment.glsl");
}

ModelBasic::~ModelBasic()
{

}

/// <summary>
/// Overriden method from Model base class - Unused in this class
/// </summary>
void ModelBasic::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

/// <summary>
/// Overriden method from Model base class - Used to draw a simple object with no lighting applied. Has the option to "copy" the position and colour of a point light
/// </summary>
void ModelBasic::drawPassTwo()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}
	
	//Bind shader
	m_modelShaderPassTwo->Bind();

	if (m_pointLightToCopy >= 0) //Copying light
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

	//Set Vertex values
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());

	setVBOAttrib(true, false, false, false, false);

	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	m_modelShaderPassTwo->Unbind();
	
}

/// <summary>
/// Indicates that this model will copy the values of a created scene pointlight (specifically its position and colour)
/// </summary>
/// <param name="index">The index of the point light in the point light vector that will be copied</param>
void ModelBasic::copyPointLight(int index)
{
	if (index <= m_localLightManager->getCurrentPointLights() && m_localLightManager->getCurrentPointLights() != 0)
	{
		m_pointLightToCopy = index;
	}
		
}
