#include "pch.h"
#include "Scene/SceneSky.h"

#include <GL/glew.h>

#include "Rendering/TextureManager.h"
#include "Rendering/OpenGLRenderer.h"
#include "Rendering/ShaderManager.h"
#include "Scene/SceneCamera.h"

SceneSky::SceneSky(const std::string& cubemapID)
	:m_cubemapID(cubemapID), m_shaderID("skyShader"), m_projectionMatrix(TheOpenGLRenderer::Instance()->getProjectionMatrix())
{
	TheShaderManager::Instance()->parseShader(m_shaderID, "res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");
	TheTextureManager::Instance()->addCubemap(cubemapID);

	// Skybox uses its own VBO and attribute system to allow the use of a custom cube
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);

}

SceneSky::~SceneSky()
{
	glDeleteBuffers(1, &m_skyboxVBO);
}

void SceneSky::drawSky()
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

	TheOpenGLRenderer::Instance()->drawCubemap(36);

	/*
		Post-draw cleanup
	*/

	temp->unbindShader();
	TheTextureManager::Instance()->getCubemapAtID(m_cubemapID)->unbindCubemap();
}
