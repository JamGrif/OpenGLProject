#include "pch.h"
#include "Rendering/Material.h"

#include "Rendering/TextureManager.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/ShaderManager.h"

#include <glm\gtc\matrix_transform.hpp>


Material::Material(const MaterialLoaderParams& pParams)
	:m_shaderID("lightingShader"), m_textureMapUsing{ false, false, false, false, false }
{
	// Go through MaterialLoaderParams and set texture values
	if (pParams.diffuseMapID != "null")
	{
		m_textureMapIDs[e_diffuseTextureSlot] = pParams.diffuseMapID;
		m_textureMapUsing[e_diffuseTextureSlot] = true;

		TheTextureManager::Instance()->parseTexture(pParams.diffuseMapID, TextureType::DIFFUSE_TEXTURE);
	}

	if (pParams.specularMapID != "null")
	{
		m_textureMapIDs[e_specularTextureSlot] = pParams.specularMapID;
		m_textureMapUsing[e_specularTextureSlot] = true;

		TheTextureManager::Instance()->parseTexture(pParams.specularMapID, TextureType::SPECULAR_TEXTURE);
	}

	if (pParams.normalMapID != "null")
	{
		m_textureMapIDs[e_normalTextureSlot] = pParams.normalMapID;
		m_textureMapUsing[e_normalTextureSlot] = true;

		TheTextureManager::Instance()->parseTexture(pParams.normalMapID, TextureType::NORMAL_TEXTURE);
	}

	if (pParams.heightMapID != "null")
	{
		m_textureMapIDs[e_heightTextureSlot] = pParams.heightMapID;
		m_textureMapUsing[e_heightTextureSlot] = true;

		TheTextureManager::Instance()->parseTexture(pParams.heightMapID, TextureType::HEIGHT_TEXTURE);
	}

	if (pParams.emissionMapID != "null")
	{
		m_textureMapIDs[e_emissionTextureSlot] = pParams.emissionMapID;
		m_textureMapUsing[e_emissionTextureSlot] = true;

		TheTextureManager::Instance()->parseTexture(pParams.emissionMapID, TextureType::EMISSION_TEXTURE);
	}

	m_normalMapNormalize = pParams.normalMapNormalize;
	m_heightMapHeight = pParams.heightMapHeight;

	// Use shaderID to read the shader
	TheShaderManager::Instance()->parseShader(m_shaderID, "res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
	m_materialShader = TheShaderManager::Instance()->getShaderAtID(m_shaderID);
}

void Material::bindMaterial(const glm::mat4& modelMat, const glm::mat4& viewMat, SceneLightManager* localLightManager, const glm::mat4& projMat, SceneCamera* pSceneCamera)
{
	// Go through each texture in material and bind them
	if (m_textureMapUsing[e_diffuseTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_diffuseTextureSlot])->bindTexture();
	}
	if (m_textureMapUsing[e_specularTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_specularTextureSlot])->bindTexture();
	}
	if (m_textureMapUsing[e_normalTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_normalTextureSlot])->bindTexture();
	}
	if (m_textureMapUsing[e_heightTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_heightTextureSlot])->bindTexture();
	}
	if (m_textureMapUsing[e_emissionTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_emissionTextureSlot])->bindTexture();
	}

	// Bind the materials shader
	m_materialShader->bindShader();

	// Set the values of the vertex shader
	m_materialShader->setUniformMatrix4fv("m_matrix", modelMat);
	m_materialShader->setUniformMatrix4fv("v_matrix", viewMat);
	m_materialShader->setUniformMatrix4fv("proj_matrix", projMat);

	// Set the values of the fragment shader
	m_materialShader->setUniform1i("material.diffuse", e_diffuseTextureSlot);
	m_materialShader->setUniform1i("material.specular", e_specularTextureSlot);
	m_materialShader->setUniform1i("material.emission", e_emissionTextureSlot);
	m_materialShader->setUniform1i("material.normal", e_normalTextureSlot);
	m_materialShader->setUniform1i("material.height", e_heightTextureSlot);
	m_materialShader->setUniform1f("material.specularShininess", 48.0f);

	m_materialShader->setUniform1i("material.normalizeTex", m_normalMapNormalize);
	m_materialShader->setUniform1i("material.usingNormal", m_textureMapUsing[e_normalTextureSlot]);
	m_materialShader->setUniform1i("material.usingEmission", m_textureMapUsing[e_emissionTextureSlot]);
	m_materialShader->setUniform1i("material.usingHeight", m_textureMapUsing[e_heightTextureSlot]);
	m_materialShader->setUniform1f("material.heightAmount", m_heightMapHeight);

	// Camera Position
	m_materialShader->setUniform3f("viewPos", pSceneCamera->getPosition());

	// Apply directional light values to shader
	if (localLightManager->getCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		std::shared_ptr<DirectionalLight> tempDirectionalLight = localLightManager->getDirectionalLight(0).lock();

		m_materialShader->setUniform3f("dLight.ambient", tempDirectionalLight->m_Ambient);
		m_materialShader->setUniform3f("dLight.diffuse", tempDirectionalLight->m_Diffuse);
		m_materialShader->setUniform3f("dLight.specular", tempDirectionalLight->m_Specular);
		m_materialShader->setUniform3f("dLight.direction", tempDirectionalLight->m_Direction);
		m_materialShader->setUniform1i("dLight.lightActive", tempDirectionalLight->m_lightActive);
	}

	// Apply point light(s) values to shader
	if (localLightManager->getCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < localLightManager->getCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			std::shared_ptr<PointLight> tempPointLight = localLightManager->getPointLight(i).lock();

			m_materialShader->setUniform3f("pLight[" + s + "].ambient", tempPointLight->m_Ambient);
			m_materialShader->setUniform3f("pLight[" + s + "].diffuse", tempPointLight->m_Diffuse);
			m_materialShader->setUniform3f("pLight[" + s + "].specular", tempPointLight->m_Specular);
			m_materialShader->setUniform3f("pLight[" + s + "].position", tempPointLight->m_Position);
			m_materialShader->setUniform1f("pLight[" + s + "].constant", tempPointLight->m_Constant);
			m_materialShader->setUniform1f("pLight[" + s + "].linear", tempPointLight->m_Linear);
			m_materialShader->setUniform1f("pLight[" + s + "].quadratic", tempPointLight->m_Quadratic);
			m_materialShader->setUniform1i("pLight[" + s + "].lightActive", tempPointLight->m_lightActive);
		}
	}

	// Apply spot light values to shader
	if (localLightManager->getCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		std::shared_ptr<SpotLight> tempSpotLight = localLightManager->getSpotLight(0).lock();

		m_materialShader->setUniform3f("sLight.ambient", tempSpotLight->m_Ambient);
		m_materialShader->setUniform3f("sLight.diffuse", tempSpotLight->m_Diffuse);
		m_materialShader->setUniform3f("sLight.specular", tempSpotLight->m_Specular);
		m_materialShader->setUniform3f("sLight.position", pSceneCamera->getPosition());
		m_materialShader->setUniform3f("sLight.direction", pSceneCamera->getFront());
		m_materialShader->setUniform1f("sLight.cutOff", glm::cos(glm::radians(tempSpotLight->m_cutOff)));
		m_materialShader->setUniform1f("sLight.outerCutOff", glm::cos(glm::radians(tempSpotLight->m_outerCutOff)));
		m_materialShader->setUniform1f("sLight.constant", tempSpotLight->m_Constant);
		m_materialShader->setUniform1f("sLight.linear", tempSpotLight->m_Linear);
		m_materialShader->setUniform1f("sLight.quadratic", tempSpotLight->m_Quadratic);
		m_materialShader->setUniform1i("sLight.lightActive", tempSpotLight->m_lightActive);
	}
}

void Material::unbindMaterial()
{
	// Go through each texture in material and unbind them
	if (m_textureMapUsing[e_diffuseTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_diffuseTextureSlot])->unbindTexture();
	}
	if (m_textureMapUsing[e_specularTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_specularTextureSlot])->unbindTexture();
	}
	if (m_textureMapUsing[e_normalTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_normalTextureSlot])->unbindTexture();
	}
	if (m_textureMapUsing[e_heightTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_heightTextureSlot])->unbindTexture();
	}
	if (m_textureMapUsing[e_emissionTextureSlot])
	{
		TheTextureManager::Instance()->getTextureAtID(m_textureMapIDs[e_emissionTextureSlot])->unbindTexture();
	}

	//m_materialShader->Unbind();
	TheShaderManager::Instance()->getShaderAtID(m_shaderID)->unbindShader();

}
