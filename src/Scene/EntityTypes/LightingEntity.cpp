#include "pch.h"
#include "Scene/EntityTypes/LightingEntity.h"

#include "Scene/SceneLightManager.h"

#include <glm\gtc\matrix_transform.hpp>

LightingEntity::LightingEntity()
	:m_modelUsingTextures{false, false, false, false, false},
	m_specularShininess(0.0f), m_normalizeTexture(false), m_heightAmount(0.5)
{
	setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
	setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");

	
	//m_collisionObject = std::make_shared<CollisionObject>();
	//affectedByGravity();
}

LightingEntity::~LightingEntity()
{
	for (auto texture : m_modelTextures)
	{
		texture->Unbind();
		texture = nullptr;
	}
	m_modelTextures.clear();
	

	//m_collisionObject = nullptr;
}

void LightingEntity::initEntity()
{
	setMatrixValues(true);
	m_modelMesh->setVertexAttributes(true, true, true, true);
	//m_collisionObject->init(m_position, m_scale, m_rotation, m_modelMesh->getFilePath());
}

void LightingEntity::updateEntity()
{
	//std::cout << m_position.y << std::endl;
	//m_collisionObject->update(m_position, m_scale, m_rotation);
	//std::cout << m_position.y << std::endl;
}

/// <summary>
/// Overridden method from Model base class - Used to render the scene to the shadow map depth texture
/// </summary>
void LightingEntity::drawPassOne()
{
	//// If no valid model or shader attached
	//if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	//{
	//	return;
	//}
	//
	//m_modelShaderPassOne->Bind();
	////m_modelShaderPassOne->setUniformMatrix4fv("lightSpaceMatrix", *EngineStatics::getLightSpaceMatrix());
	//m_modelShaderPassOne->setUniformMatrix4fv("model", m_mMat);
	//
	//setVBOAttrib(true, false, false, false, false);
	//
	//// Draw
	//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_modelMesh->getIndices().size()), GL_UNSIGNED_INT, 0);
	//
	//m_modelShaderPassOne->Unbind();
}

/// <summary>
/// Overridden method from Model base class - Used to light an object by taking values from all active lights, binding texture maps and drawing the object
/// </summary>
void LightingEntity::drawPassTwo()
{
	// If no valid model or shader attached
	if (!m_modelMesh || !m_modelShaderPassTwo)
		return;
	
	// Bind shader
	m_modelShaderPassTwo->Bind();

	/*
		Set Vertex values
	*/

	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix",		m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix",		m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix",	*m_localProjectionMatrix);

	/*
		Set Fragment values
	*/

	// Apply directional light values to shader
	if (m_localLightManager->getCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		DirectionalLight* tempDirectionalLight = m_localLightManager->getDirectionalLight(0);

		m_modelShaderPassTwo->setUniform3f("dLight.ambient",		tempDirectionalLight->Ambient);
		m_modelShaderPassTwo->setUniform3f("dLight.diffuse",		tempDirectionalLight->Diffuse);
		m_modelShaderPassTwo->setUniform3f("dLight.specular",		tempDirectionalLight->Specular);
		m_modelShaderPassTwo->setUniform3f("dLight.direction",		tempDirectionalLight->Direction);
		m_modelShaderPassTwo->setUniform1i("dLight.lightActive",	tempDirectionalLight->lightActive);
	}

	// Apply point light(s) values to shader
	if (m_localLightManager->getCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < m_localLightManager->getCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			PointLight* tempPointLight = m_localLightManager->getPointLight(i);

			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].ambient",		tempPointLight->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].diffuse",		tempPointLight->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].specular",	tempPointLight->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].position",	tempPointLight->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[" + s + "].constant",	tempPointLight->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[" + s + "].linear",		tempPointLight->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[" + s + "].quadratic",	tempPointLight->Quadratic);
			m_modelShaderPassTwo->setUniform1i("pLight[" + s + "].lightActive", tempPointLight->lightActive);
		}
	}

	// Apply spot light values to shader
	if (m_localLightManager->getCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		SpotLight* tempSpotLight = m_localLightManager->getSpotLight(0);

		m_modelShaderPassTwo->setUniform3f("sLight.ambient",			tempSpotLight->Ambient);
		m_modelShaderPassTwo->setUniform3f("sLight.diffuse",			tempSpotLight->Diffuse);
		m_modelShaderPassTwo->setUniform3f("sLight.specular",			tempSpotLight->Specular);
		m_modelShaderPassTwo->setUniform3f("sLight.position",			EngineStatics::getCamera()->getPosition());
		m_modelShaderPassTwo->setUniform3f("sLight.direction",			EngineStatics::getCamera()->getFront());
		m_modelShaderPassTwo->setUniform1f("sLight.cutOff",				glm::cos(glm::radians(tempSpotLight->cutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.outerCutOff",		glm::cos(glm::radians(tempSpotLight->outerCutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.constant",			tempSpotLight->Constant);
		m_modelShaderPassTwo->setUniform1f("sLight.linear",				tempSpotLight->Linear);
		m_modelShaderPassTwo->setUniform1f("sLight.quadratic",			tempSpotLight->Quadratic);
		m_modelShaderPassTwo->setUniform1i("sLight.lightActive",		tempSpotLight->lightActive);
	}

	// Material properties
	m_modelShaderPassTwo->setUniform1i("material.diffuse",				e_diffuseTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.specular",				e_specularTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.emission",				e_emissionTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.normal",				e_normalTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.height",				e_heightTextureSlot);
	m_modelShaderPassTwo->setUniform1f("material.specularShininess",	m_specularShininess);

	m_modelShaderPassTwo->setUniform1i("material.normalizeTex",			m_normalizeTexture);
	m_modelShaderPassTwo->setUniform1i("material.usingNormal",			m_modelUsingTextures[e_normalTextureSlot]);
	m_modelShaderPassTwo->setUniform1i("material.usingEmission",		m_modelUsingTextures[e_emissionTextureSlot]);
	m_modelShaderPassTwo->setUniform1i("material.usingHeight",			m_modelUsingTextures[e_heightTextureSlot]);
	m_modelShaderPassTwo->setUniform1f("material.heightAmount",			m_heightAmount);

	// Camera Position
	m_modelShaderPassTwo->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());
	
	/*
		Bind textures to pipeline
	*/

	for (auto texture : m_modelTextures)
	{
		texture->Bind();
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

	for (auto texture : m_modelTextures)
	{
		texture->Unbind();
	}
	
	m_modelShaderPassTwo->Unbind();
}

/// <summary>
/// Assigns specified texture to the model to be used for a diffuse map
/// </summary>
/// <param name="texturePath"></param>
void LightingEntity::setDiffuseTexture(const std::string& texturePath)
{
	std::shared_ptr<OpenGLTexture> text = TextureManager::retrieveTextureObject(texturePath);
	text->setTextureSlot(e_diffuseTextureSlot);
	m_modelTextures.push_back(text);

	m_modelUsingTextures[e_diffuseTextureSlot] = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a specular map
/// </summary>
/// <param name="texturePath"></param>
/// <param name="shininessAmount">Shiniess of the specular texture in the fragment shader</param>
void LightingEntity::setSpecularTexture(const std::string& texturePath, float shininessAmount)
{
	std::shared_ptr<OpenGLTexture> text = TextureManager::retrieveTextureObject(texturePath);
	text->setTextureSlot(e_specularTextureSlot);
	m_modelTextures.push_back(text);

	m_modelUsingTextures[e_specularTextureSlot] = true;

	m_specularShininess = shininessAmount;
}

/// <summary>
/// Assigns specified texture to the model to be used for an emission map
/// </summary>
/// <param name="texturePath"></param>
void LightingEntity::setEmissionTexture(const std::string& texturePath)
{
	std::shared_ptr<OpenGLTexture> text = TextureManager::retrieveTextureObject(texturePath);
	text->setTextureSlot(e_emissionTextureSlot);
	m_modelTextures.push_back(text);

	m_modelUsingTextures[e_emissionTextureSlot] = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a normal map
/// </summary>
/// <param name="texturePath"></param>
/// <param name="normalize">Should the texture be normalized in the fragment shader</param>
void LightingEntity::setNormalTexture(const std::string& texturePath, bool normalize)
{
	std::shared_ptr<OpenGLTexture> text = TextureManager::retrieveTextureObject(texturePath);
	text->setTextureSlot(e_normalTextureSlot);
	m_modelTextures.push_back(text);

	m_modelUsingTextures[e_normalTextureSlot] = true;

	m_normalizeTexture = normalize;
}

/// <summary>
/// Assigns specified texture to the model to be used for a height map
/// </summary>
/// <param name="texturePath"></param>
void LightingEntity::setHeightTexture(const std::string& texturePath, float heightAmount)
{
	std::shared_ptr<OpenGLTexture> text = TextureManager::retrieveTextureObject(texturePath);
	text->setTextureSlot(e_heightTextureSlot);
	m_modelTextures.push_back(text);

	m_modelUsingTextures[e_heightTextureSlot] = true;

	m_heightAmount = heightAmount;
}

void LightingEntity::affectedByGravity(bool gravity)
{
	//m_collisionObject->affectedByGravity(gravity);
}
