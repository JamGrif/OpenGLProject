#include "ModelLighting.h"

#include "../LightManager.h"

ModelLighting::ModelLighting()
	:m_modelDiffuseTexture(nullptr), m_modelSpecularTexture(nullptr), m_modelEmissionTexture(nullptr), 
	m_modelNormalTexture(nullptr), m_modelHeightTexture(nullptr), m_shininess(48.0f), m_normalizeTexture(false), 
	m_usingEmission(false), m_usingNormal(false), m_usingHeight(false), m_heightAmount(0.5)
{
	setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
	setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
}

ModelLighting::~ModelLighting()
{
	if (m_modelDiffuseTexture)
	{
		m_modelDiffuseTexture->Unbind(e_diffuseTextureSlot);
		m_modelDiffuseTexture = nullptr;
	}

	if (m_modelSpecularTexture)
	{
		m_modelSpecularTexture->Unbind(e_specularTextureSlot);
		m_modelSpecularTexture = nullptr;
	}

	if (m_modelEmissionTexture)
	{
		m_modelEmissionTexture->Unbind(e_emissionTextureSlot);
		m_modelEmissionTexture = nullptr;
	}

	if (m_modelNormalTexture)
	{
		m_modelNormalTexture->Unbind(e_normalTextureSlot);
		m_modelNormalTexture = nullptr;
	}

	if (m_modelHeightTexture)
	{
		m_modelHeightTexture->Unbind(e_heightTextureSlot);
		m_modelHeightTexture = nullptr;
	}
}

/// <summary>
/// Overridden method from Model base class - Used to render the scene to the shadow map depth texture
/// </summary>
void ModelLighting::drawPassOne()
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
void ModelLighting::drawPassTwo()
{
	// If no valid model or shader attached
	if (!m_modelMesh || !m_modelShaderPassTwo)
	{
		return;
	}

	// Bind shader
	m_modelShaderPassTwo->Bind();

	/*
		Set Vertex values
	*/

	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix",		m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix",		m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix",	m_localProjectionMatrix);

	/*
		Set Fragment values
	*/

	// Apply directional light values to shader
	if (m_localLightManager->getCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		m_modelShaderPassTwo->setUniform3f("dLight.ambient",		m_localLightManager->getDirectionalLight(0)->Ambient);
		m_modelShaderPassTwo->setUniform3f("dLight.diffuse",		m_localLightManager->getDirectionalLight(0)->Diffuse);
		m_modelShaderPassTwo->setUniform3f("dLight.specular",		m_localLightManager->getDirectionalLight(0)->Specular);
		m_modelShaderPassTwo->setUniform3f("dLight.direction",		m_localLightManager->getDirectionalLight(0)->Direction);
		m_modelShaderPassTwo->setUniform1i("dLight.lightActive",	m_localLightManager->getDirectionalLight(0)->lightActive);
	}

	// Apply point light(s) values to shader
	if (m_localLightManager->getCurrentPointLights() > 0) // Ensure a point light exists
	{
		unsigned int x = m_localLightManager->getCurrentPointLights();

		for (unsigned int i = 0; i < m_localLightManager->getCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);

			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].ambient",		m_localLightManager->getPointLight(i)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].diffuse",		m_localLightManager->getPointLight(i)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].specular",	m_localLightManager->getPointLight(i)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[" + s + "].position",	m_localLightManager->getPointLight(i)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[" + s + "].constant",	m_localLightManager->getPointLight(i)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[" + s + "].linear",		m_localLightManager->getPointLight(i)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[" + s + "].quadratic",	m_localLightManager->getPointLight(i)->Quadratic);
			m_modelShaderPassTwo->setUniform1i("pLight[" + s + "].lightActive", m_localLightManager->getPointLight(i)->lightActive);
		}
	}

	// Apply spot light values to shader
	if (m_localLightManager->getCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		m_modelShaderPassTwo->setUniform3f("sLight.ambient",		m_localLightManager->getSpotLight(0)->Ambient);
		m_modelShaderPassTwo->setUniform3f("sLight.diffuse",		m_localLightManager->getSpotLight(0)->Diffuse);
		m_modelShaderPassTwo->setUniform3f("sLight.specular",		m_localLightManager->getSpotLight(0)->Specular);
		m_modelShaderPassTwo->setUniform3f("sLight.position",		EngineStatics::getCamera()->getPosition());
		m_modelShaderPassTwo->setUniform3f("sLight.direction",		EngineStatics::getCamera()->getFront());
		m_modelShaderPassTwo->setUniform1f("sLight.cutOff",			glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->cutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.outerCutOff",	glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->outerCutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.constant",		m_localLightManager->getSpotLight(0)->Constant);
		m_modelShaderPassTwo->setUniform1f("sLight.linear",			m_localLightManager->getSpotLight(0)->Linear);
		m_modelShaderPassTwo->setUniform1f("sLight.quadratic",		m_localLightManager->getSpotLight(0)->Quadratic);
		m_modelShaderPassTwo->setUniform1i("sLight.lightActive",	m_localLightManager->getSpotLight(0)->lightActive);
	}

	// Material properties
	m_modelShaderPassTwo->setUniform1i("material.diffuse",		e_diffuseTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.specular",		e_specularTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.emission",		e_emissionTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.normal",		e_normalTextureSlot);
	m_modelShaderPassTwo->setUniform1i("material.height",		e_heightTextureSlot);
	m_modelShaderPassTwo->setUniform1f("material.shininess",	m_shininess);

	m_modelShaderPassTwo->setUniform1i("material.normalizeTex",		m_normalizeTexture);
	m_modelShaderPassTwo->setUniform1i("material.usingNormal",		m_usingNormal);
	m_modelShaderPassTwo->setUniform1i("material.usingEmission",	m_usingEmission);
	m_modelShaderPassTwo->setUniform1i("material.usingHeight",		m_usingHeight);
	m_modelShaderPassTwo->setUniform1f("material.heightAmount",		m_heightAmount);

	// Camera Position
	m_modelShaderPassTwo->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());
	
	/*
		Bind textures to pipeline
	*/

	if (m_modelDiffuseTexture)
	{
		m_modelDiffuseTexture->Bind(e_diffuseTextureSlot);
	}

	if (m_modelSpecularTexture)
	{
		m_modelSpecularTexture->Bind(e_specularTextureSlot);
	}

	if (m_modelEmissionTexture)
	{
		m_modelEmissionTexture->Bind(e_emissionTextureSlot);
	}

	if (m_modelNormalTexture)
	{
		m_modelNormalTexture->Bind(e_normalTextureSlot);
	}

	if (m_modelHeightTexture)
	{
		m_modelHeightTexture->Bind(e_heightTextureSlot);
	}

	/*
		Bind VBOs and vertex attributes
	*/

	setVBOAttrib(true, true, true, true, true); 
	//if (m_modelNormalTexture != nullptr)
	//{
	//}
	//else
	//{
	//	setVBOAttrib(true, true, true, false, false); //No use for tangents / bitangents if no normal map
	//}
	
	/*
		Draw
	*/

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_modelMesh->getIndices().size()), GL_UNSIGNED_INT, 0);

	/*
		Post-draw cleanup
	*/

	if (m_modelDiffuseTexture)
	{
		m_modelDiffuseTexture->Unbind(e_diffuseTextureSlot);
	}

	if (m_modelSpecularTexture)
	{
		m_modelSpecularTexture->Unbind(e_specularTextureSlot);
	}

	if (m_modelEmissionTexture)
	{
		m_modelEmissionTexture->Unbind(e_emissionTextureSlot);
	}

	if (m_modelNormalTexture)
	{
		m_modelNormalTexture->Unbind(e_normalTextureSlot);
	}

	if (m_modelHeightTexture)
	{
		m_modelHeightTexture->Unbind(e_heightTextureSlot);
	}
	
	m_modelShaderPassTwo->Unbind();
}

/// <summary>
/// Assigns specified texture to the model to be used for a diffuse map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setDiffuseTexture(const std::string& texturePath)
{
	m_modelDiffuseTexture = TextureManager::retrieveTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for a specular map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setSpecularTexture(const std::string& texturePath)
{
	m_modelSpecularTexture = TextureManager::retrieveTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for an emission map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setEmissionTexture(const std::string& texturePath)
{
	m_modelEmissionTexture = TextureManager::retrieveTexture(texturePath);
	m_usingEmission = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a normal map
/// </summary>
/// <param name="texturePath"></param>
/// <param name="normalize">Should the texture be normalized in the fragment shader</param>
void ModelLighting::setNormalTexture(const std::string& texturePath, bool normalize)
{
	m_modelNormalTexture = TextureManager::retrieveTexture(texturePath);
	m_normalizeTexture = normalize;
	m_usingNormal = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a height map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setHeightTexture(const std::string& texturePath, float heightAmount)
{
	m_modelHeightTexture = TextureManager::retrieveTexture(texturePath);
	m_usingHeight = true;
	m_heightAmount = heightAmount;
}

/// <summary>
/// Sets the specular maps shininess in the fragment shader
/// </summary>
/// <param name="value"></param>
void ModelLighting::setSpecularShiniess(float value)
{
	m_shininess = value;
}
