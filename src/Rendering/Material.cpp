#include "pch.h"
#include "Rendering/Material.h"

#include "Rendering/Resource/Manager/TextureManager.h"
#include "Rendering/Resource/Manager/ShaderManager.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/OpenGLRenderer.h"

#include <glm\gtc\matrix_transform.hpp>

static const char* NULL_TEXTURE = "null";

Material::Material(const MaterialLoaderParams& pParams)
	:m_shaderID("lightingShader"),
	m_textureMapUsing{ false, false, false, false, false },
	m_pAppProjectionMatrix(OpenGLRenderer::Instance()->GetProjectionMatrix())
{
	// Go through MaterialLoaderParams and set texture values
	if (pParams.diffuseMapID != NULL_TEXTURE)
	{
		m_textureMapIDs[static_cast<int>(TextureType::DIFFUSE)] = pParams.diffuseMapID;
		m_textureMapUsing[static_cast<int>(TextureType::DIFFUSE)] = true;

		TheTextureManager::Instance()->AddTexture(pParams.diffuseMapID, TextureType::DIFFUSE);
	}

	if (pParams.specularMapID != NULL_TEXTURE)
	{
		m_textureMapIDs[static_cast<int>(TextureType::SPECULAR)] = pParams.specularMapID;
		m_textureMapUsing[static_cast<int>(TextureType::SPECULAR)] = true;

		TheTextureManager::Instance()->AddTexture(pParams.specularMapID, TextureType::SPECULAR);
	}

	if (pParams.normalMapID != NULL_TEXTURE)
	{
		m_textureMapIDs[static_cast<int>(TextureType::NORMAL)] = pParams.normalMapID;
		m_textureMapUsing[static_cast<int>(TextureType::NORMAL)] = true;

		TheTextureManager::Instance()->AddTexture(pParams.normalMapID, TextureType::NORMAL);
	}

	if (pParams.heightMapID != NULL_TEXTURE)
	{
		m_textureMapIDs[static_cast<int>(TextureType::HEIGHT)] = pParams.heightMapID;
		m_textureMapUsing[static_cast<int>(TextureType::HEIGHT)] = true;

		TheTextureManager::Instance()->AddTexture(pParams.heightMapID, TextureType::HEIGHT);
	}

	if (pParams.emissionMapID != NULL_TEXTURE)
	{
		m_textureMapIDs[static_cast<int>(TextureType::EMISSION)] = pParams.emissionMapID;
		m_textureMapUsing[static_cast<int>(TextureType::EMISSION)] = true;

		TheTextureManager::Instance()->AddTexture(pParams.emissionMapID, TextureType::EMISSION);
	}

	m_bNormalMapNormalize = pParams.normalMapNormalize;
	m_heightMapHeight = pParams.heightMapHeight;

	// Use shaderID to read the shader
	TheShaderManager::Instance()->AddShader(m_shaderID, "res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
}


Material::~Material()
{
}

/// <summary>
/// Binds the material's textures and shader to the OpenGL context and sets all uniform values for the shader
/// </summary>
void Material::BindMaterial(const glm::mat4& modelMat)
{
	// Bind each available texture
	if (m_textureMapUsing[static_cast<int>(TextureType::DIFFUSE)])
		TheTextureManager::Instance()->BindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::DIFFUSE)]);
	
	if (m_textureMapUsing[static_cast<int>(TextureType::SPECULAR)])
		TheTextureManager::Instance()->BindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::SPECULAR)]);
	
	if (m_textureMapUsing[static_cast<int>(TextureType::NORMAL)])
		TheTextureManager::Instance()->BindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::NORMAL)]);
	
	if (m_textureMapUsing[static_cast<int>(TextureType::HEIGHT)])
		TheTextureManager::Instance()->BindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::HEIGHT)]);
	
	if (m_textureMapUsing[static_cast<int>(TextureType::EMISSION)])
		TheTextureManager::Instance()->BindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::EMISSION)]);

	// Bind the materials shader
	TheShaderManager::Instance()->BindShaderAtID(m_shaderID);

	std::shared_ptr<SceneCamera> tempSceneCamera = m_pSceneCamera.lock();
	std::shared_ptr<SceneLightManager> tempLightManager = m_pSceneLightManager.lock();

	// Set the values of the vertex shader
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "m_matrix", modelMat);
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "v_matrix", tempSceneCamera->GetViewMatrix());
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "proj_matrix", m_pAppProjectionMatrix);

	// Set the values of the fragment shader
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.diffuse", static_cast<int>(TextureType::DIFFUSE));
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.specular", static_cast<int>(TextureType::SPECULAR));
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.emission", static_cast<int>(TextureType::EMISSION));
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.normal", static_cast<int>(TextureType::NORMAL));
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.height", static_cast<int>(TextureType::HEIGHT));

	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.specularShininess", 48.0f);
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.normalizeTex", m_bNormalMapNormalize);

	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.usingEmission", m_textureMapUsing[static_cast<int>(TextureType::EMISSION)]);
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.usingNormal", m_textureMapUsing[static_cast<int>(TextureType::NORMAL)]);
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.usingHeight", m_textureMapUsing[static_cast<int>(TextureType::HEIGHT)]);

	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "material.heightAmount", m_heightMapHeight);

	// Camera Position
	TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "viewPos", tempSceneCamera->GetPosition());

	// Apply directional light values to shader
	if (tempLightManager->GetCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		std::shared_ptr<DirectionalLight> tempDirectionalLight = tempLightManager->GetDirectionalLight(0).lock();

		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "dLight.ambient", tempDirectionalLight->m_ambient);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "dLight.diffuse", tempDirectionalLight->m_diffuse);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "dLight.specular", tempDirectionalLight->m_specular);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "dLight.direction", tempDirectionalLight->m_direction);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "dLight.lightActive", tempDirectionalLight->m_bLightActive);
	}

	// Apply point light(s) values to shader
	if (tempLightManager->GetCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < tempLightManager->GetCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			std::shared_ptr<PointLight> tempPointLight = tempLightManager->GetPointLight(i).lock();

			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].ambient", tempPointLight->m_ambient);
			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].diffuse", tempPointLight->m_diffuse);
			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].specular", tempPointLight->m_specular);

			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].position", tempPointLight->m_position);
			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].constant", tempPointLight->m_constant);
			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].linear", tempPointLight->m_linear);
			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].quadratic", tempPointLight->m_quadratic);
			TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "pLight[" + s + "].lightActive", tempPointLight->m_bLightActive);
		}
	}

	// Apply spot light values to shader
	if (tempLightManager->GetCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		std::shared_ptr<SpotLight> tempSpotLight = tempLightManager->GetSpotLight(0).lock();

		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.ambient", tempSpotLight->m_ambient);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.diffuse", tempSpotLight->m_diffuse);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.specular", tempSpotLight->m_specular);

		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.position", tempSceneCamera->GetPosition());
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.direction", tempSceneCamera->GetFront());

		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.cutOff", glm::cos(glm::radians(tempSpotLight->m_cutOff)));
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.outerCutOff", glm::cos(glm::radians(tempSpotLight->m_outerCutOff)));

		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.constant", tempSpotLight->m_constant);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.linear", tempSpotLight->m_linear);
		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.quadratic", tempSpotLight->m_quadratic);

		TheShaderManager::Instance()->SetUniformAtID(m_shaderID, "sLight.lightActive", tempSpotLight->m_bLightActive);
	}
}

/// <summary>
/// Unbinds the material's textures and shader from the OpenGL context
/// </summary>
void Material::UnbindMaterial()
{
	// Bind each available texture
	if (m_textureMapUsing[static_cast<int>(TextureType::DIFFUSE)])
		TheTextureManager::Instance()->UnbindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::DIFFUSE)]);

	if (m_textureMapUsing[static_cast<int>(TextureType::SPECULAR)])
		TheTextureManager::Instance()->UnbindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::SPECULAR)]);

	if (m_textureMapUsing[static_cast<int>(TextureType::NORMAL)])
		TheTextureManager::Instance()->UnbindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::NORMAL)]);

	if (m_textureMapUsing[static_cast<int>(TextureType::HEIGHT)])
		TheTextureManager::Instance()->UnbindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::HEIGHT)]);

	if (m_textureMapUsing[static_cast<int>(TextureType::EMISSION)])
		TheTextureManager::Instance()->UnbindTextureAtID(m_textureMapIDs[static_cast<int>(TextureType::EMISSION)]);
	
	TheShaderManager::Instance()->UnbindShaderAtID(m_shaderID);
}


void Material::SetScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera)
{
	m_pSceneLightManager = pSceneLightManager;
	m_pSceneCamera = pSceneCamera;
}
