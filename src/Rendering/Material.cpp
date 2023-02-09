#include "pch.h"
#include "Rendering/Material.h"

#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/OpenGLRenderer.h"

#include "glm\gtc\matrix_transform.hpp"

static const char* NULL_TEXTURE = "null";

Material::Material(const MaterialLoaderParams& pParams)
	:m_shaderID("lightingShader"),
	m_textureMapUsing{ false, false, false, false, false },
	m_pAppProjectionMatrix(OpenGLRenderer::Get()->GetProjectionMatrix())
{
	// Go through MaterialLoaderParams and set each texture type that is used
	for (int i = static_cast<int>(TextureType::START_OF_TEXTURETYPE); i != static_cast<int>(TextureType::END_OF_TEXTURETYPE); i++)
	{
		// Check if texture type is used in material
		if (pParams.textureMapIDs[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i] != NULL_TEXTURE)
		{
			// Set texture ID
			m_textureMapIDs[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i] = pParams.textureMapIDs[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i];
			m_textureMapUsing[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i] = true;

			// Create Texture resource with textureMapID
			TextureManager::Get()->AddResource(pParams.textureMapIDs[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i]);
			TextureManager::Get()->GetResourceAtID(pParams.textureMapIDs[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i])->setTextureType(static_cast<TextureType>(static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i));
		}
	}

	m_bNormalMapNormalize = pParams.normalMapNormalize;
	m_heightMapHeight = pParams.heightMapHeight;

	// Create shader resource with shaderID
	ShaderManager::Get()->AddResource(m_shaderID, "res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
}


Material::~Material()
{
}

/// <summary>
/// Bind material's textures and shader to the OpenGL context and sets all uniform values for the shader
/// </summary>
void Material::BindMaterial(const glm::mat4& modelMat)
{
	// Bind each used texture
	for (int i = static_cast<int>(TextureType::START_OF_TEXTURETYPE); i != static_cast<int>(TextureType::END_OF_TEXTURETYPE); i++)
	{
		if (m_textureMapUsing[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i])
			TextureManager::Get()->BindResourceAtID(m_textureMapIDs[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i]);
	}

	// Bind the materials shader
	ShaderManager::Get()->BindResourceAtID(m_shaderID);

	std::shared_ptr<SceneCamera> tempSceneCamera = m_pSceneCamera.lock();
	std::shared_ptr<SceneLightManager> tempLightManager = m_pSceneLightManager.lock();

	// Set the values of the vertex shader
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("m_matrix", modelMat);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("v_matrix", tempSceneCamera->GetViewMatrix());
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("proj_matrix", m_pAppProjectionMatrix);

	// Set the values of the fragment shader
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.diffuse", static_cast<int>(TextureType::DIFFUSE));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.specular", static_cast<int>(TextureType::SPECULAR));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.emission", static_cast<int>(TextureType::EMISSION));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.normal", static_cast<int>(TextureType::NORMAL));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.height", static_cast<int>(TextureType::HEIGHT));

	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.specularShininess", 48.0f);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.normalizeTex", m_bNormalMapNormalize);

	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.usingEmission", m_textureMapUsing[static_cast<int>(TextureType::EMISSION)]);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.usingNormal", m_textureMapUsing[static_cast<int>(TextureType::NORMAL)]);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.usingHeight", m_textureMapUsing[static_cast<int>(TextureType::HEIGHT)]);

	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.heightAmount", m_heightMapHeight);

	// Camera Position
	const Vector3D cameraPosition{ tempSceneCamera->GetPosition().x, tempSceneCamera->GetPosition().y, tempSceneCamera->GetPosition().z };
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("viewPos", cameraPosition);

	// Apply directional light values to shader
	if (tempLightManager->GetCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		std::shared_ptr<DirectionalLight> tempDirectionalLight = tempLightManager->GetDirectionalLight(0).lock();

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.ambient", tempDirectionalLight->m_ambient);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.diffuse", tempDirectionalLight->m_diffuse);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.specular", tempDirectionalLight->m_specular);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.direction", tempDirectionalLight->m_direction);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.lightActive", tempDirectionalLight->m_bLightActive);
	}

	// Apply point light(s) values to shader
	if (tempLightManager->GetCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < tempLightManager->GetCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			std::shared_ptr<PointLight> tempPointLight = tempLightManager->GetPointLight(i).lock();

			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].ambient", tempPointLight->m_ambient);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].diffuse", tempPointLight->m_diffuse);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].specular", tempPointLight->m_specular);

			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].position", tempPointLight->m_position);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].constant", tempPointLight->m_constant);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].linear", tempPointLight->m_linear);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].quadratic", tempPointLight->m_quadratic);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].lightActive", tempPointLight->m_bLightActive);
		}
	}

	// Apply spot light values to shader
	if (tempLightManager->GetCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		std::shared_ptr<SpotLight> tempSpotLight = tempLightManager->GetSpotLight(0).lock();
		Vector3D cameraFront{ tempSceneCamera->GetFront().x, tempSceneCamera->GetFront().y, tempSceneCamera->GetFront().z };

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.ambient", tempSpotLight->m_ambient);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.diffuse", tempSpotLight->m_diffuse);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.specular", tempSpotLight->m_specular);

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.position", cameraPosition);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.direction", cameraFront);

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.cutOff", glm::cos(glm::radians(tempSpotLight->m_cutOff)));
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.outerCutOff", glm::cos(glm::radians(tempSpotLight->m_outerCutOff)));

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.constant", tempSpotLight->m_constant);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.linear", tempSpotLight->m_linear);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.quadratic", tempSpotLight->m_quadratic);

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.lightActive", tempSpotLight->m_bLightActive);
	}
}

/// <summary>
/// Unbinds the material's textures and shader from the OpenGL context
/// </summary>
void Material::UnbindMaterial()
{
	// Unbind each available texture
	for (int i = static_cast<int>(TextureType::START_OF_TEXTURETYPE); i != static_cast<int>(TextureType::END_OF_TEXTURETYPE); i++)
	{
		if (m_textureMapUsing[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i])
			TextureManager::Get()->UnbindResourceAtID(m_textureMapIDs[static_cast<int>(TextureType::START_OF_TEXTURETYPE) + i]);
	}
	
	ShaderManager::Get()->UnbindResourceAtID(m_shaderID);
}

/// <summary>
/// Sets the scene specific pointers of the materials
/// </summary>
void Material::SetScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera)
{
	m_pSceneLightManager = pSceneLightManager;
	m_pSceneCamera = pSceneCamera;
}
