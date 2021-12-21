#include "ModelSky.h"

ModelSky::ModelSky(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation)
{

	m_skyTexture = TextureManager::retrieveCubeMap();

	//setShaderOne--------
	setShaderTwo("res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");

	//Skybox uses its own VBO and attribute system to allow the use of a custom cube
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);
}

ModelSky::~ModelSky()
{
	glDeleteBuffers(1, &m_skyboxVBO);
}

/// <summary>
/// Overridden method from Model base class - Unused in this class
/// </summary>
void ModelSky::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}
}

/// <summary>
/// Overridden method from Model base class - Used to draw the skybox
/// </summary>
void ModelSky::drawPassTwo()
{
	//If no valid model or shader attached
	if (m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	m_modelShaderPassTwo->Bind();

	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", glm::mat4(glm::mat3(EngineStatics::getCamera()->getViewMatrix())));
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());
	m_modelShaderPassTwo->setUniform1i("sky", 0);

	m_skyTexture->Bind();

	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);

	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Draw - disables writing to the depth buffer
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);

	m_modelShaderPassTwo->Unbind();
	m_skyTexture->Unbind();
}
