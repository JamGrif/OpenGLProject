#include "pch.h"
#include "Rendering/Material.h"

#include "Rendering/TextureManager.h"
#include "core/EngineStatics.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/ShaderManager.h"

#include <glm\gtc\matrix_transform.hpp>


Material::Material(const MaterialLoaderParams& pParams)
{
	m_materialID = pParams.materialID;

	m_diffuseMapID = pParams.diffuseMapID;
	if (m_diffuseMapID != "null")
	{
		TheTextureManager::Instance()->parseTexture(m_diffuseMapID, TextureType::DIFFUSE_TEXTURE);
	}

	m_specularMapID = pParams.specularMapID;
	if (m_specularMapID != "null")
	{
		TheTextureManager::Instance()->parseTexture(m_specularMapID, TextureType::SPECULAR_TEXTURE);
	}

	m_normalMapID = pParams.normalMapID;
	if (m_normalMapID != "null")
	{
		TheTextureManager::Instance()->parseTexture(m_normalMapID, TextureType::NORMAL_TEXTURE);
		m_bUsingNormal = true;
	}

	m_normalMapNormalize = pParams.normalMapNormalize;

	m_heightMapID = pParams.heightMapID;
	if (m_heightMapID != "null")
	{
		TheTextureManager::Instance()->parseTexture(m_heightMapID, TextureType::HEIGHT_TEXTURE);
		m_bUsingHeight = true;
	}

	m_heightMapHeight = pParams.heightMapHeight;

	m_emissionMapID = pParams.emissionMapID;
	if (m_emissionMapID != "null")
	{
		TheTextureManager::Instance()->parseTexture(m_emissionMapID, TextureType::EMISSION_TEXTURE);
		m_bUsingEmission = true;
	}


	m_shaderID = "lightingShader";
	TheShaderManager::Instance()->parseShader(m_shaderID, "res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");

	//PRINT_TRACE("created material with diffusepath of {0}", m_diffuseMapID);
}

Material::~Material()
{

}

void Material::bindMaterial(const glm::mat4& modelMat, const glm::mat4& viewMat, std::shared_ptr<SceneLightManager> localLightManager, const glm::mat4& projMat)
{
	// Go through each texture in material and bind them
	if (m_diffuseMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_diffuseMapID)->bindTexture();
	}
	if (m_specularMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_specularMapID)->bindTexture();
	}
	if (m_normalMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_normalMapID)->bindTexture();
	}
	if (m_heightMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_heightMapID)->bindTexture();
	}
	if (m_emissionMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_emissionMapID)->bindTexture();
	}

	// Bind the materials shader
	//m_materialShader->Bind();
	// Save Shader pointer to avoid numerous calls to ShaderManager
	Shader* temp = TheShaderManager::Instance()->getShaderAtID(m_shaderID);
	temp->bindShader();

	// Set the values of the shader
	/*
		Set Vertex values
	*/

	temp->setUniformMatrix4fv("m_matrix", modelMat);
	temp->setUniformMatrix4fv("v_matrix", viewMat);
	temp->setUniformMatrix4fv("proj_matrix", projMat);

	/*
		Set Fragment values
	*/

	// Apply directional light values to shader
	if (localLightManager->getCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		std::shared_ptr<DirectionalLight> tempDirectionalLight = localLightManager->getDirectionalLight(0).lock();

		temp->setUniform3f("dLight.ambient", tempDirectionalLight->m_Ambient);
		temp->setUniform3f("dLight.diffuse", tempDirectionalLight->m_Diffuse);
		temp->setUniform3f("dLight.specular", tempDirectionalLight->m_Specular);
		temp->setUniform3f("dLight.direction", tempDirectionalLight->m_Direction);
		temp->setUniform1i("dLight.lightActive", tempDirectionalLight->m_lightActive);
	}

	// Apply point light(s) values to shader
	if (localLightManager->getCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < localLightManager->getCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			std::shared_ptr<PointLight> tempPointLight = localLightManager->getPointLight(i).lock();

			temp->setUniform3f("pLight[" + s + "].ambient", tempPointLight->m_Ambient);
			temp->setUniform3f("pLight[" + s + "].diffuse", tempPointLight->m_Diffuse);
			temp->setUniform3f("pLight[" + s + "].specular", tempPointLight->m_Specular);
			temp->setUniform3f("pLight[" + s + "].position", tempPointLight->m_Position);
			temp->setUniform1f("pLight[" + s + "].constant", tempPointLight->m_Constant);
			temp->setUniform1f("pLight[" + s + "].linear", tempPointLight->m_Linear);
			temp->setUniform1f("pLight[" + s + "].quadratic", tempPointLight->m_Quadratic);
			temp->setUniform1i("pLight[" + s + "].lightActive", tempPointLight->m_lightActive);
		}
	}

	// Apply spot light values to shader
	if (localLightManager->getCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		std::shared_ptr<SpotLight> tempSpotLight = localLightManager->getSpotLight(0).lock();

		temp->setUniform3f("sLight.ambient", tempSpotLight->m_Ambient);
		temp->setUniform3f("sLight.diffuse", tempSpotLight->m_Diffuse);
		temp->setUniform3f("sLight.specular", tempSpotLight->m_Specular);
		temp->setUniform3f("sLight.position", EngineStatics::getCamera()->getPosition());
		temp->setUniform3f("sLight.direction", EngineStatics::getCamera()->getFront());
		temp->setUniform1f("sLight.cutOff", glm::cos(glm::radians(tempSpotLight->m_cutOff)));
		temp->setUniform1f("sLight.outerCutOff", glm::cos(glm::radians(tempSpotLight->m_outerCutOff)));
		temp->setUniform1f("sLight.constant", tempSpotLight->m_Constant);
		temp->setUniform1f("sLight.linear", tempSpotLight->m_Linear);
		temp->setUniform1f("sLight.quadratic", tempSpotLight->m_Quadratic);
		temp->setUniform1i("sLight.lightActive", tempSpotLight->m_lightActive);
	}

	// Material properties
	temp->setUniform1i("material.diffuse", e_diffuseTextureSlot);
	temp->setUniform1i("material.specular", e_specularTextureSlot);
	temp->setUniform1i("material.emission", e_emissionTextureSlot);
	temp->setUniform1i("material.normal", e_normalTextureSlot);
	temp->setUniform1i("material.height", e_heightTextureSlot);
	temp->setUniform1f("material.specularShininess", 48.0f);

	temp->setUniform1i("material.normalizeTex", m_normalMapNormalize);
	temp->setUniform1i("material.usingNormal", m_bUsingNormal);
	temp->setUniform1i("material.usingEmission", m_bUsingEmission);
	temp->setUniform1i("material.usingHeight", m_bUsingHeight);
	temp->setUniform1f("material.heightAmount", m_heightMapHeight);

	// Camera Position
	temp->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());

}

void Material::unbindMaterial()
{
	// Go through each texture in material and unbind them
	if (m_diffuseMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_diffuseMapID)->unbindTexture();
	}
	if (m_specularMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_specularMapID)->unbindTexture();
	}
	if (m_normalMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_normalMapID)->unbindTexture();
	}
	if (m_heightMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_heightMapID)->unbindTexture();
	}
	if (m_emissionMapID != "null")
	{
		TheTextureManager::Instance()->getTextureAtID(m_emissionMapID)->unbindTexture();
	}

	//m_materialShader->Unbind();
	TheShaderManager::Instance()->getShaderAtID(m_shaderID)->unbindShader();

}
