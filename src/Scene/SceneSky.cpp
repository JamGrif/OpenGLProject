#include "pch.h"
#include "Scene/SceneSky.h"

#include <GL/glew.h>

#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Rendering/OpenGLRenderer.h"
#include "Scene/SceneCamera.h"

SceneSky::SceneSky(const ResourceID& cubemapID)
	:m_cubemapID(cubemapID), m_shaderID("skyShader"), m_projectionMatrix(TheOpenGLRenderer::Get()->GetProjectionMatrix())
{
	ShaderManager::Get()->AddResource(m_shaderID, "res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");

	// Skybox uses its own VBO and attribute system to allow the use of a custom cube
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);
}

SceneSky::~SceneSky()
{
	glDeleteBuffers(1, &m_skyboxVBO);
}

/// <summary>
/// Render the scene sky by drawing the cubemap
/// </summary>
void SceneSky::DrawSky()
{	
	// Bind shader
	ShaderManager::Get()->BindResourceAtID(m_shaderID);

	// Bind Vertex values
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("v_matrix", glm::mat4(glm::mat3(m_pSceneCamera.lock()->GetViewMatrix())));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("proj_matrix", m_projectionMatrix);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sky", 0);

	CubemapManager::Get()->BindResourceAtID(m_cubemapID);

	// Bind VBOs and vertex attributes	
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// Draw
	TheOpenGLRenderer::Get()->DrawCubemap(36);

	// Post-draw cleanup
	ShaderManager::Get()->UnbindResourceAtID(m_shaderID);
	CubemapManager::Get()->UnbindResourceAtID(m_cubemapID);
}
