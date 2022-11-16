#include "pch.h"
#include "Scene/SceneSky.h"

#include <GL/glew.h>

#include "Rendering/TextureManager.h"
#include "Scene/SceneCamera.h"
#include "Rendering/OpenGLRenderer.h"
#include "Rendering/ShaderManager.h"


SceneSky::SceneSky(const std::string& cubemapID)
	:m_projectionMatrix(TheOpenGLRenderer::Instance()->getProjectionMatrix())
{
	//std::cout << "scenesky created" << std::endl;
	//setSkyboxTexture(object.skyboxTexture);

	//setShaderOne--------
	//setShaderTwo("res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");

	//std::cout << skyshader.get() << std::endl;
	//skyshader = OpenGLShaderManager::retrieveShader("res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");
	m_shaderID = "skyShader";
	TheShaderManager::Instance()->parseShader(m_shaderID, "res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");
	//std::cout << skyshader.get() << std::endl;

	m_cubemapID = cubemapID;
	TheTextureManager::Instance()->createCubemap(cubemapID);

	// Skybox uses its own VBO and attribute system to allow the use of a custom cube
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);

}

SceneSky::~SceneSky()
{
	//m_skyTexture = nullptr;
	glDeleteBuffers(1, &m_skyboxVBO);
}

void SceneSky::initEntity()
{
	//m_skyTexture->setTextureSlot(0);
}

void SceneSky::updateEntity()
{
}

/// <summary>
/// Overridden method from BaseEntity class - Unused in this class
/// </summary>
void SceneSky::drawPassOne()
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
void SceneSky::drawPassTwo()
{	
	// Bind shader
	Shader* temp = TheShaderManager::Instance()->getShaderAtID(m_shaderID);
	temp->bindShader();

	/*
		Bind Vertex values
	*/

	temp->setUniformMatrix4fv("v_matrix", glm::mat4(glm::mat3(m_pSceneCamera->getViewMatrix())));
	temp->setUniformMatrix4fv("proj_matrix", m_projectionMatrix);
	temp->setUniform1i("sky", 0);

	//m_skyTexture->Bind();
	TheTextureManager::Instance()->getCubemapAtID(m_cubemapID)->bindCubemap();

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

	//m_localRenderer->drawCubemap(36);
	TheOpenGLRenderer::Instance()->drawCubemap(36);

	/*
		Post-draw cleanup
	*/

	//m_modelMesh->Unbind();
	temp->unbindShader();
	//m_skyTexture->Unbind();
	TheTextureManager::Instance()->getCubemapAtID(m_cubemapID)->unbindCubemap();
}
