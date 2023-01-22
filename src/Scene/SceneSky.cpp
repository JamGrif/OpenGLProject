#include "pch.h"
#include "Scene/SceneSky.h"

#include <GL/glew.h>

#include "Rendering/TextureManager.h"
#include "Rendering/CubemapManager.h"
#include "Rendering/OpenGLRenderer.h"
#include "Rendering/ShaderManager.h"
#include "Scene/SceneCamera.h"

SceneSky::SceneSky(const std::string& cubemapID)
	:m_cubemapID(cubemapID), m_shaderID("skyShader"), m_projectionMatrix(TheOpenGLRenderer::Instance()->GetProjectionMatrix())
{
	TheShaderManager::Instance()->AddShader(m_shaderID, "res/shaders/sky-vertex.glsl", "res/shaders/sky-fragment.glsl");
	//TheCubemapManager::Instance()->addCubemap(cubemapID);

	// Skybox uses its own VBO and attribute system to allow the use of a custom cube
	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);
}

SceneSky::~SceneSky()
{
	glDeleteBuffers(1, &m_skyboxVBO);
}

void SceneSky::DrawSky()
{	
	// Bind shader
	TheShaderManager::Instance()->BindShaderAtID(m_shaderID);

	// Bind Vertex values
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "v_matrix", glm::mat4(glm::mat3(m_pSceneCamera.lock()->GetViewMatrix())));
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "proj_matrix", m_projectionMatrix);
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sky", 0);

	TheCubemapManager::Instance()->BindCubemapAtID(m_cubemapID);

	// Bind VBOs and vertex attributes	
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);

	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// Draw
	TheOpenGLRenderer::Instance()->DrawCubemap(36);

	// Post-draw cleanup
	TheShaderManager::Instance()->UnbindShaderAtID(m_shaderID);
	TheCubemapManager::Instance()->UnbindCubemapAtID(m_cubemapID);
}
