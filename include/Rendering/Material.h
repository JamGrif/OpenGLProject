#pragma once

#include "Scene/SceneLightManager.h"
#include "Rendering/ShaderManager.h"

// Numbers correspond to texture order in shader
enum textureSlotNumber
{
	e_diffuseTextureSlot	= 0,
	e_specularTextureSlot	= 1,
	e_emissionTextureSlot	= 2,
	e_normalTextureSlot		= 3,
	e_heightTextureSlot		= 4
};

struct MaterialLoaderParams
{
	std::string materialID;

	std::string diffuseMapID;
	std::string diffuseMapPath;

	std::string specularMapID;
	std::string specularMapPath;

	std::string normalMapID;
	std::string normalMapPath;

	float		normalMapNormalize;

	std::string heightMapID;
	std::string heightMapPath;

	float		heightMapHeight;

	std::string emissionMapID;
	std::string emissionMapPath;

};

class Material
{
public:
	Material(const MaterialLoaderParams& pParams);
	~Material();

	void bindMaterial(const glm::mat4& modelMat, const glm::mat4& viewMat, std::shared_ptr<SceneLightManager> localLightManager, const glm::mat4& projMat);
	void unbindMaterial();

private:

	std::string m_materialID;

	std::string m_diffuseMapID;
	std::string m_specularMapID;
	std::string m_normalMapID;

	float m_normalMapNormalize;

	std::string m_heightMapID;

	float m_heightMapHeight;

	std::string m_emissionMapID;

	//std::shared_ptr<OpenGLShader> m_materialShader;

	bool m_bUsingNormal = false;
	bool m_bUsingHeight = false;
	bool m_bUsingEmission = false;

	std::string m_shaderID;

};
