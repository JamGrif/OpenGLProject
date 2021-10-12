#include "ModelEnvironment.h"

ModelEnvironment::ModelEnvironment(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation), m_usingReflection(false), m_usingRefraction(false)
{
	//setShaderOne--------
	setShaderTwo("res/shaders/environmentMapping-vertex.glsl", "res/shaders/environmentMapping-fragment.glsl");

	m_skyTexture = TextureManager::retrieveCubeMap();
	//setMesh("res/meshes/heart.obj");
}

ModelEnvironment::~ModelEnvironment()
{
	m_skyTexture = nullptr;
}

/// <summary>
/// Overriden method from Model base class - Unused in this class
/// </summary>
void ModelEnvironment::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

/// <summary>
/// Overriden method from Model base class - Used for environment mapping by taking the skybox texture and mapping it to the texture
/// </summary>
void ModelEnvironment::drawPassTwo()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	//If not using either environment mapping types, stop drawing object
	if (!m_usingReflection && !m_usingRefraction)
	{
		return;
	}

	//Bind shader
	m_modelShaderPassTwo->Bind();

	//Set Vertex values
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());

	m_modelShaderPassTwo->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());
	m_modelShaderPassTwo->setUniform1i("sky", 0);
	m_modelShaderPassTwo->setUniform1i("usingReflection", m_usingReflection);
	m_modelShaderPassTwo->setUniform1i("usingRefraction", m_usingRefraction);

	m_skyTexture->Bind();

	setVBOAttrib(true, true, false, false, false);

	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	m_modelShaderPassTwo->Unbind();
	m_skyTexture->Unbind();
}

void ModelEnvironment::toggleReflection(bool value)
{
	m_usingReflection = value;
}

void ModelEnvironment::toggleRefraction(bool value)
{
	m_usingRefraction = value;
}
