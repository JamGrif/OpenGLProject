#include "pch.h"
#include "Rendering/Material.h"

#include "Rendering/TextureManager.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/ShaderManager.h"

#include <glm\gtc\matrix_transform.hpp>


Material::Material(const MaterialLoaderParams& pParams)
	:m_shaderID("lightingShader"), m_textureMapUsing{ false, false, false, false, false },
	m_pSceneLightManager(nullptr), m_pSceneCamera(nullptr), m_pAppProjectionMatrix(nullptr)
{
	//PRINT_TRACE("material created");
	
	// Go through MaterialLoaderParams and set texture values
	if (pParams.diffuseMapID != "null")
	{
		m_textureMapIDs[e_diffuseTextureSlot] = pParams.diffuseMapID;
		m_textureMapUsing[e_diffuseTextureSlot] = true;

		TheTextureManager::Instance()->addTexture(pParams.diffuseMapID, TextureType::DIFFUSE_TEXTURE);
	}

	if (pParams.specularMapID != "null")
	{
		m_textureMapIDs[e_specularTextureSlot] = pParams.specularMapID;
		m_textureMapUsing[e_specularTextureSlot] = true;

		TheTextureManager::Instance()->addTexture(pParams.specularMapID, TextureType::SPECULAR_TEXTURE);
	}

	if (pParams.normalMapID != "null")
	{
		m_textureMapIDs[e_normalTextureSlot] = pParams.normalMapID;
		m_textureMapUsing[e_normalTextureSlot] = true;

		TheTextureManager::Instance()->addTexture(pParams.normalMapID, TextureType::NORMAL_TEXTURE);
	}

	if (pParams.heightMapID != "null")
	{
		m_textureMapIDs[e_heightTextureSlot] = pParams.heightMapID;
		m_textureMapUsing[e_heightTextureSlot] = true;

		TheTextureManager::Instance()->addTexture(pParams.heightMapID, TextureType::HEIGHT_TEXTURE);
	}

	if (pParams.emissionMapID != "null")
	{
		m_textureMapIDs[e_emissionTextureSlot] = pParams.emissionMapID;
		m_textureMapUsing[e_emissionTextureSlot] = true;

		TheTextureManager::Instance()->addTexture(pParams.emissionMapID, TextureType::EMISSION_TEXTURE);
	}

	m_normalMapNormalize = pParams.normalMapNormalize;
	m_heightMapHeight = pParams.heightMapHeight;

	// Use shaderID to read the shader
	TheShaderManager::Instance()->parseShader(m_shaderID, "res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
	m_materialShader = TheShaderManager::Instance()->getShaderAtID(m_shaderID);
}


Material::~Material()
{
	//PRINT_TRACE("material destroyed");
}

void Material::bindMaterial(const glm::mat4& modelMat)
{
	// Go through each texture in material and bind them
	if (m_textureMapUsing[e_diffuseTextureSlot])
	{
		TheTextureManager::Instance()->bindTextureAtID(m_textureMapIDs[e_diffuseTextureSlot]);
	}
	if (m_textureMapUsing[e_specularTextureSlot])
	{
		TheTextureManager::Instance()->bindTextureAtID(m_textureMapIDs[e_specularTextureSlot]);
	}
	if (m_textureMapUsing[e_normalTextureSlot])
	{
		TheTextureManager::Instance()->bindTextureAtID(m_textureMapIDs[e_normalTextureSlot]);
	}
	if (m_textureMapUsing[e_heightTextureSlot])
	{
		TheTextureManager::Instance()->bindTextureAtID(m_textureMapIDs[e_heightTextureSlot]);
	}
	if (m_textureMapUsing[e_emissionTextureSlot])
	{
		TheTextureManager::Instance()->bindTextureAtID(m_textureMapIDs[e_emissionTextureSlot]);
	}

	// Bind the materials shader
	m_materialShader->bindShader();

	// Set the values of the vertex shader
	m_materialShader->setUniformMatrix4fv("m_matrix", modelMat);
	m_materialShader->setUniformMatrix4fv("v_matrix", m_pSceneCamera->getViewMatrix());
	m_materialShader->setUniformMatrix4fv("proj_matrix", *m_pAppProjectionMatrix);

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
	m_materialShader->setUniform3f("viewPos", m_pSceneCamera->getPosition());

	// Apply directional light values to shader
	if (m_pSceneLightManager->getCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		std::shared_ptr<DirectionalLight> tempDirectionalLight = m_pSceneLightManager->getDirectionalLight(0).lock();

		m_materialShader->setUniform3f("dLight.ambient", tempDirectionalLight->m_ambient);
		m_materialShader->setUniform3f("dLight.diffuse", tempDirectionalLight->m_diffuse);
		m_materialShader->setUniform3f("dLight.specular", tempDirectionalLight->m_specular);
		m_materialShader->setUniform3f("dLight.direction", tempDirectionalLight->m_Direction);
		m_materialShader->setUniform1i("dLight.lightActive", tempDirectionalLight->m_bLightActive);
	}

	// Apply point light(s) values to shader
	if (m_pSceneLightManager->getCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < m_pSceneLightManager->getCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			std::shared_ptr<PointLight> tempPointLight = m_pSceneLightManager->getPointLight(i).lock();

			m_materialShader->setUniform3f("pLight[" + s + "].ambient", tempPointLight->m_ambient);
			m_materialShader->setUniform3f("pLight[" + s + "].diffuse", tempPointLight->m_diffuse);
			m_materialShader->setUniform3f("pLight[" + s + "].specular", tempPointLight->m_specular);
			m_materialShader->setUniform3f("pLight[" + s + "].position", tempPointLight->m_Position);
			m_materialShader->setUniform1f("pLight[" + s + "].constant", tempPointLight->m_Constant);
			m_materialShader->setUniform1f("pLight[" + s + "].linear", tempPointLight->m_Linear);
			m_materialShader->setUniform1f("pLight[" + s + "].quadratic", tempPointLight->m_Quadratic);
			m_materialShader->setUniform1i("pLight[" + s + "].lightActive", tempPointLight->m_bLightActive);
		}
	}

	// Apply spot light values to shader
	if (m_pSceneLightManager->getCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		std::shared_ptr<SpotLight> tempSpotLight = m_pSceneLightManager->getSpotLight(0).lock();

		m_materialShader->setUniform3f("sLight.ambient", tempSpotLight->m_ambient);
		m_materialShader->setUniform3f("sLight.diffuse", tempSpotLight->m_diffuse);
		m_materialShader->setUniform3f("sLight.specular", tempSpotLight->m_specular);
		m_materialShader->setUniform3f("sLight.position", m_pSceneCamera->getPosition());
		m_materialShader->setUniform3f("sLight.direction", m_pSceneCamera->getFront());
		m_materialShader->setUniform1f("sLight.cutOff", glm::cos(glm::radians(tempSpotLight->m_cutOff)));
		m_materialShader->setUniform1f("sLight.outerCutOff", glm::cos(glm::radians(tempSpotLight->m_outerCutOff)));
		m_materialShader->setUniform1f("sLight.constant", tempSpotLight->m_Constant);
		m_materialShader->setUniform1f("sLight.linear", tempSpotLight->m_Linear);
		m_materialShader->setUniform1f("sLight.quadratic", tempSpotLight->m_Quadratic);
		m_materialShader->setUniform1i("sLight.lightActive", tempSpotLight->m_bLightActive);
	}
}

void Material::unbindMaterial()
{
	// Go through each texture in material and unbind them
	if (m_textureMapUsing[e_diffuseTextureSlot])
	{
		TheTextureManager::Instance()->unbindTextureAtID(m_textureMapIDs[e_diffuseTextureSlot]);
	}
	if (m_textureMapUsing[e_specularTextureSlot])
	{
		TheTextureManager::Instance()->unbindTextureAtID(m_textureMapIDs[e_specularTextureSlot]);
	}
	if (m_textureMapUsing[e_normalTextureSlot])
	{
		TheTextureManager::Instance()->unbindTextureAtID(m_textureMapIDs[e_normalTextureSlot]);
	}
	if (m_textureMapUsing[e_heightTextureSlot])
	{
		TheTextureManager::Instance()->unbindTextureAtID(m_textureMapIDs[e_heightTextureSlot]);
	}
	if (m_textureMapUsing[e_emissionTextureSlot])
	{
		TheTextureManager::Instance()->unbindTextureAtID(m_textureMapIDs[e_emissionTextureSlot]);
	}

	TheShaderManager::Instance()->getShaderAtID(m_shaderID)->unbindShader();
}

void Material::setScenePointers(SceneLightManager* pSceneLightManager, SceneCamera* pSceneCamera, const glm::mat4& projMat)
{
	m_pSceneLightManager = pSceneLightManager;
	m_pSceneCamera = pSceneCamera;
	m_pAppProjectionMatrix = &projMat;
}
