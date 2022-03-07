#include "models/ModelLighting.h"

#include "LightManager.h"

ModelLighting::ModelLighting()
	:m_modelTextures{nullptr, nullptr, nullptr, nullptr, nullptr},
	m_modelUsingTextures{false, false, false, false, false},
	m_specularShininess(48.0f), m_normalizeTexture(false), m_heightAmount(0.5)
{
	setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
	setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
}

ModelLighting::~ModelLighting()
{
	if (m_modelTextures[e_diffuseTextureSlot])
	{
		m_modelTextures[e_diffuseTextureSlot]->Unbind(e_diffuseTextureSlot);
		m_modelTextures[e_diffuseTextureSlot] = nullptr;
	}

	if (m_modelTextures[e_specularTextureSlot])
	{
		m_modelTextures[e_specularTextureSlot]->Unbind(e_specularTextureSlot);
		m_modelTextures[e_specularTextureSlot] = nullptr;
	}

	if (m_modelTextures[e_emissionTextureSlot])
	{
		m_modelTextures[e_emissionTextureSlot]->Unbind(e_emissionTextureSlot);
		m_modelTextures[e_emissionTextureSlot] = nullptr;
	}

	if (m_modelTextures[e_normalTextureSlot])
	{
		m_modelTextures[e_normalTextureSlot]->Unbind(e_normalTextureSlot);
		m_modelTextures[e_normalTextureSlot] = nullptr;
	}

	if (m_modelTextures[e_heightTextureSlot])
	{
		m_modelTextures[e_heightTextureSlot]->Unbind(e_heightTextureSlot);
		m_modelTextures[e_heightTextureSlot] = nullptr;
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

	if (m_modelUsingTextures[e_diffuseTextureSlot])
		m_modelTextures[e_diffuseTextureSlot]->Bind(e_diffuseTextureSlot);
	

	if (m_modelUsingTextures[e_specularTextureSlot])
		m_modelTextures[e_specularTextureSlot]->Bind(e_specularTextureSlot);
	
	if (m_modelUsingTextures[e_emissionTextureSlot])
		m_modelTextures[e_emissionTextureSlot]->Bind(e_emissionTextureSlot);
	
	if (m_modelUsingTextures[e_normalTextureSlot])
		m_modelTextures[e_normalTextureSlot]->Bind(e_normalTextureSlot);
	
	if (m_modelUsingTextures[e_heightTextureSlot])
		m_modelTextures[e_heightTextureSlot]->Bind(e_heightTextureSlot);
	
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

	if (m_modelUsingTextures[e_diffuseTextureSlot])
		m_modelTextures[e_diffuseTextureSlot]->Unbind(e_diffuseTextureSlot);
	
	if (m_modelUsingTextures[e_specularTextureSlot])
		m_modelTextures[e_specularTextureSlot]->Unbind(e_specularTextureSlot);
	
	if (m_modelUsingTextures[e_emissionTextureSlot])
		m_modelTextures[e_emissionTextureSlot]->Unbind(e_emissionTextureSlot);
	
	if (m_modelUsingTextures[e_normalTextureSlot])
		m_modelTextures[e_normalTextureSlot]->Unbind(e_normalTextureSlot);
	
	if (m_modelUsingTextures[e_heightTextureSlot])
		m_modelTextures[e_heightTextureSlot]->Unbind(e_heightTextureSlot);
	
	m_modelShaderPassTwo->Unbind();
}

/// <summary>
/// Assigns specified texture to the model to be used for a diffuse map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setDiffuseTexture(const std::string& texturePath)
{
	m_modelTextures[e_diffuseTextureSlot] = TextureManager::retrieveTexture(texturePath);
	m_modelUsingTextures[e_diffuseTextureSlot] = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a specular map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setSpecularTexture(const std::string& texturePath)
{
	m_modelTextures[e_specularTextureSlot] = TextureManager::retrieveTexture(texturePath);
	m_modelUsingTextures[e_specularTextureSlot] = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for an emission map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setEmissionTexture(const std::string& texturePath)
{
	m_modelTextures[e_emissionTextureSlot] = TextureManager::retrieveTexture(texturePath);
	m_modelUsingTextures[e_emissionTextureSlot] = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a normal map
/// </summary>
/// <param name="texturePath"></param>
/// <param name="normalize">Should the texture be normalized in the fragment shader</param>
void ModelLighting::setNormalTexture(const std::string& texturePath, bool normalize)
{
	m_modelTextures[e_normalTextureSlot] = TextureManager::retrieveTexture(texturePath);
	m_modelUsingTextures[e_normalTextureSlot] = true;

	m_normalizeTexture = normalize;
}

/// <summary>
/// Assigns specified texture to the model to be used for a height map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setHeightTexture(const std::string& texturePath, float heightAmount)
{
	m_modelTextures[e_heightTextureSlot] = TextureManager::retrieveTexture(texturePath);
	m_modelUsingTextures[e_heightTextureSlot] = true;

	m_heightAmount = heightAmount;
}

/// <summary>
/// Sets the specular maps shininess in the fragment shader
/// </summary>
/// <param name="value"></param>
void ModelLighting::setSpecularShiniess(float value)
{
	m_specularShininess = value;
}
