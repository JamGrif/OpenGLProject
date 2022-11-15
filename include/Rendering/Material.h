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
	std::string specularMapID;
	std::string normalMapID;
	std::string heightMapID;
	std::string emissionMapID;

	bool		normalMapNormalize;
	float		heightMapHeight;
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

	bool m_normalMapNormalize;

	std::string m_heightMapID;

	float m_heightMapHeight;

	std::string m_emissionMapID;

	bool m_bUsingNormal = false;
	bool m_bUsingHeight = false;
	bool m_bUsingEmission = false;

	std::string m_shaderID;

};
