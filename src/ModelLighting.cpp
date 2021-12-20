#include "ModelLighting.h"

ModelLighting::ModelLighting(glm::vec3 position, glm::vec3 rotation)
	:Model(position, rotation), m_modelDiffuseTexture(nullptr), m_modelSpecularTexture(nullptr),
	m_modelEmissionTexture(nullptr), m_modelNormalTexture(nullptr), m_modelHeightTexture(nullptr), m_shininess(48.0f),
	m_normalizeTexture(false), m_usingEmission(false), m_usingNormal(false), m_usingHeight(false), m_heightAmount(0.5)
{

	setShaderOne("res/shaders/lightingPassOne-vertex.glsl", "res/shaders/lightingPassOne-fragment.glsl");
	setShaderTwo("res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");
}

ModelLighting::~ModelLighting()
{
	if (m_modelDiffuseTexture != nullptr)
	{
		m_modelDiffuseTexture = nullptr;
	}

	if (m_modelSpecularTexture != nullptr)
	{
		m_modelSpecularTexture = nullptr;
	}

	if (m_modelEmissionTexture != nullptr)
	{
		m_modelEmissionTexture = nullptr;
	}

	if (m_modelNormalTexture != nullptr)
	{
		m_modelNormalTexture = nullptr;
	}

	if (m_modelHeightTexture != nullptr)
	{
		m_modelHeightTexture = nullptr;
	}
}

/// <summary>
/// Overriden method from Model base class - Used to render the scene to the shadow map depth texture
/// </summary>
void ModelLighting::drawPassOne()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassOne == nullptr)
	{
		return;
	}

	m_modelShaderPassOne->Bind();
	m_modelShaderPassOne->setUniformMatrix4fv("lightSpaceMatrix", *EngineStatics::getLightSpaceMatrix());
	m_modelShaderPassOne->setUniformMatrix4fv("model", m_mMat);

	setVBOAttrib(true, false, false, false, false);

	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	m_modelShaderPassOne->Unbind();

}

/// <summary>
/// Overriden method from Model base class - Used to light an object by taking values from all active lights, binding texture maps and drawing the object
/// </summary>
void ModelLighting::drawPassTwo()
{
	//If no valid model or shader attached
	if (m_modelMesh == nullptr || m_modelShaderPassTwo == nullptr)
	{
		return;
	}

	//Bind shader
	m_modelShaderPassTwo->Bind();

	//Set Vertex values
	m_modelShaderPassTwo->setUniformMatrix4fv("m_matrix", m_mMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("v_matrix", m_vMat);
	m_modelShaderPassTwo->setUniformMatrix4fv("proj_matrix", *EngineStatics::getProjectionMatrix());

	/*
		Set Fragment values
	*/

	
	//Ensure a directional light exists
	if (m_localLightManager->getCurrentDirectionalLights() > 0)
	{
		m_modelShaderPassTwo->setUniform3f("dLight.ambient", m_localLightManager->getDirectionalLight(0)->Ambient);
		m_modelShaderPassTwo->setUniform3f("dLight.diffuse", m_localLightManager->getDirectionalLight(0)->Diffuse);
		m_modelShaderPassTwo->setUniform3f("dLight.specular", m_localLightManager->getDirectionalLight(0)->Specular);
		m_modelShaderPassTwo->setUniform3f("dLight.direction", m_localLightManager->getDirectionalLight(0)->Direction);
		m_modelShaderPassTwo->setUniform1i("dLight.lightActive", m_localLightManager->getDirectionalLight(0)->lightActive);
	}

	//Ensure a point light exists
	if (m_localLightManager->getCurrentPointLights() > 0)
	{
		//Point lights - (I know a better way of this can be done, but when I tried doing it in a for loop by converting and combining the index to a string I encountered performance issues when launching project)
		if (m_localLightManager->getCurrentPointLights() >= 1)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[0].ambient", m_localLightManager->getPointLight(0)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[0].diffuse", m_localLightManager->getPointLight(0)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[0].specular", m_localLightManager->getPointLight(0)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[0].position", m_localLightManager->getPointLight(0)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[0].constant", m_localLightManager->getPointLight(0)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[0].linear", m_localLightManager->getPointLight(0)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[0].quadratic", m_localLightManager->getPointLight(0)->Quadratic);
			m_modelShaderPassTwo->setUniform1i("pLight[0].lightActive", m_localLightManager->getPointLight(0)->lightActive);
		}

		if (m_localLightManager->getCurrentPointLights() >= 2)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[1].ambient", m_localLightManager->getPointLight(1)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[1].diffuse", m_localLightManager->getPointLight(1)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[1].specular", m_localLightManager->getPointLight(1)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[1].position", m_localLightManager->getPointLight(1)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[1].constant", m_localLightManager->getPointLight(1)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[1].linear", m_localLightManager->getPointLight(1)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[1].quadratic", m_localLightManager->getPointLight(1)->Quadratic);
			m_modelShaderPassTwo->setUniform1i("pLight[1].lightActive", m_localLightManager->getPointLight(1)->lightActive);
		}

		if (m_localLightManager->getCurrentPointLights() >= 3)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[2].ambient", m_localLightManager->getPointLight(2)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[2].diffuse", m_localLightManager->getPointLight(2)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[2].specular", m_localLightManager->getPointLight(2)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[2].position", m_localLightManager->getPointLight(2)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[2].constant", m_localLightManager->getPointLight(2)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[2].linear", m_localLightManager->getPointLight(2)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[2].quadratic", m_localLightManager->getPointLight(2)->Quadratic);
			m_modelShaderPassTwo->setUniform1i("pLight[2].lightActive", m_localLightManager->getPointLight(2)->lightActive);
		}

		if (m_localLightManager->getCurrentPointLights() >= 4)
		{
			m_modelShaderPassTwo->setUniform3f("pLight[3].ambient", m_localLightManager->getPointLight(3)->Ambient);
			m_modelShaderPassTwo->setUniform3f("pLight[3].diffuse", m_localLightManager->getPointLight(3)->Diffuse);
			m_modelShaderPassTwo->setUniform3f("pLight[3].specular", m_localLightManager->getPointLight(3)->Specular);
			m_modelShaderPassTwo->setUniform3f("pLight[3].position", m_localLightManager->getPointLight(3)->Position);
			m_modelShaderPassTwo->setUniform1f("pLight[3].constant", m_localLightManager->getPointLight(3)->Constant);
			m_modelShaderPassTwo->setUniform1f("pLight[3].linear", m_localLightManager->getPointLight(3)->Linear);
			m_modelShaderPassTwo->setUniform1f("pLight[3].quadratic", m_localLightManager->getPointLight(3)->Quadratic);
			m_modelShaderPassTwo->setUniform1i("pLight[3].lightActive", m_localLightManager->getPointLight(3)->lightActive);
		}
	}

	//Ensure a spot light exists
	if (m_localLightManager->getCurrentSpotLights() > 0)
	{
		m_modelShaderPassTwo->setUniform3f("sLight.ambient", m_localLightManager->getSpotLight(0)->Ambient);
		m_modelShaderPassTwo->setUniform3f("sLight.diffuse", m_localLightManager->getSpotLight(0)->Diffuse);
		m_modelShaderPassTwo->setUniform3f("sLight.specular", m_localLightManager->getSpotLight(0)->Specular);
		m_modelShaderPassTwo->setUniform3f("sLight.position", EngineStatics::getCamera()->getPosition());
		m_modelShaderPassTwo->setUniform3f("sLight.direction", EngineStatics::getCamera()->getFront());
		m_modelShaderPassTwo->setUniform1f("sLight.cutOff", glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->cutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.outerCutOff", glm::cos(glm::radians(m_localLightManager->getSpotLight(0)->outerCutOff)));
		m_modelShaderPassTwo->setUniform1f("sLight.constant", m_localLightManager->getSpotLight(0)->Constant);
		m_modelShaderPassTwo->setUniform1f("sLight.linear", m_localLightManager->getSpotLight(0)->Linear);
		m_modelShaderPassTwo->setUniform1f("sLight.quadratic", m_localLightManager->getSpotLight(0)->Quadratic);
		m_modelShaderPassTwo->setUniform1i("sLight.lightActive", m_localLightManager->getSpotLight(0)->lightActive);
	}


	//Material properties


	m_modelShaderPassTwo->setUniform1i("material.diffuse", 0);
	m_modelShaderPassTwo->setUniform1i("material.specular", 1);
	m_modelShaderPassTwo->setUniform1i("material.emission", 2);
	m_modelShaderPassTwo->setUniform1i("material.normal", 3);
	m_modelShaderPassTwo->setUniform1i("material.height", 4);
	//m_modelShaderPassTwo->setUniform1i("material.depthMap", 5); //depthMap contains shows depth information of scene for shadowing
	m_modelShaderPassTwo->setUniform1f("material.shininess", m_shininess);

	m_modelShaderPassTwo->setUniform1i("material.normalizeTex", m_normalizeTexture);
	m_modelShaderPassTwo->setUniform1i("material.usingNormal", m_usingNormal);
	m_modelShaderPassTwo->setUniform1i("material.usingEmission", m_usingEmission);
	m_modelShaderPassTwo->setUniform1i("material.usingHeight", m_usingHeight);
	m_modelShaderPassTwo->setUniform1f("material.heightAmount", m_heightAmount);

	//Camera Position
	m_modelShaderPassTwo->setUniform3f("viewPos", EngineStatics::getCamera()->getPosition());
	//m_modelShaderPassTwo->setUniformMatrix4fv("lightSpaceMatrix", *EngineStatics::getLightSpaceMatrix()); //Used for shadowing and is used to find the location of the light in fragment shader


	//Bind textures to pipeline
	if (m_modelDiffuseTexture != nullptr)
	{
		m_modelDiffuseTexture->Bind(0);
	}

	if (m_modelSpecularTexture != nullptr)
	{
		m_modelSpecularTexture->Bind(1);
	}

	if (m_modelEmissionTexture != nullptr)
	{
		m_modelEmissionTexture->Bind(2);
	}

	if (m_modelNormalTexture != nullptr)
	{
		m_modelNormalTexture->Bind(3);
	}

	if (m_modelHeightTexture != nullptr)
	{
		m_modelHeightTexture->Bind(4);
	}

	//Unused depthmap which gets sent to shader
	//glActiveTexture(GL_TEXTURE0+5);
	//glBindTexture(GL_TEXTURE_2D, *EngineStatics::getDepthMap());

	/*
		Bind VBOs and vertex attributes
	*/

	if (m_modelNormalTexture != nullptr)
	{
		setVBOAttrib(true, true, true, true, true); 
	}
	else
	{
		setVBOAttrib(true, true, true, false, false); //No use for tangents / bitangents if no normal map
	}
	
	//Draw
	glDrawElements(GL_TRIANGLES, m_modelMesh->getIndices().size(), GL_UNSIGNED_INT, 0);

	if (m_modelDiffuseTexture != nullptr)
	{
		m_modelDiffuseTexture->Unbind();
	}

	if (m_modelSpecularTexture != nullptr)
	{
		m_modelSpecularTexture->Unbind();
	}

	if (m_modelEmissionTexture != nullptr)
	{
		m_modelEmissionTexture->Unbind();
	}

	if (m_modelNormalTexture != nullptr)
	{
		m_modelNormalTexture->Unbind();
	}

	if (m_modelHeightTexture != nullptr)
	{
		m_modelHeightTexture->Unbind();
	}
	
	m_modelShaderPassTwo->Unbind();


	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, 0);

}

/// <summary>
/// Assigns specified texture to the model to be used for a diffuse map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setDiffuseTexture(std::string texturePath)
{
	m_modelDiffuseTexture = TextureManager::retrieveTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for a specular map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setSpecularTexture(std::string texturePath)
{
	m_modelSpecularTexture = TextureManager::retrieveTexture(texturePath);
}

/// <summary>
/// Assigns specified texture to the model to be used for an emission map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setEmissionTexture(std::string texturePath)
{
	m_modelEmissionTexture = TextureManager::retrieveTexture(texturePath);
	m_usingEmission = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a normal map
/// </summary>
/// <param name="texturePath"></param>
/// <param name="normalize">Should the texture be normalized in the fragment shader</param>
void ModelLighting::setNormalTexture(std::string texturePath, bool normalize)
{
	m_modelNormalTexture = TextureManager::retrieveTexture(texturePath);
	m_normalizeTexture = normalize;
	m_usingNormal = true;
}

/// <summary>
/// Assigns specified texture to the model to be used for a height map
/// </summary>
/// <param name="texturePath"></param>
void ModelLighting::setHeightTexture(std::string texturePath, float heightAmount)
{
	m_modelHeightTexture = TextureManager::retrieveTexture(texturePath);
	m_usingHeight = true;
	m_heightAmount = heightAmount;
}

void ModelLighting::setSpecularShiniess(float value)
{
	m_shininess = value;
}
