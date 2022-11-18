#pragma once

#include "glm/mat4x4.hpp"

class Shader;
class SceneCamera;
class SceneLightManager;

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
	std::string diffuseMapID;		// ID for TextureManager for diffuse map
	std::string specularMapID;		// ID for TextureManager for specular map
	std::string normalMapID;		// ID for TextureManager for normal map
	std::string heightMapID;		// ID for TextureManager for height map
	std::string emissionMapID;		// ID for TextureManager for emission map

	bool		normalMapNormalize;	// Should the normal map be normalized in the fragment shader?
	float		heightMapHeight;	// Height to draw the height map within fragment shader
};

class Material
{
public:
	Material(const MaterialLoaderParams& pParams);
	~Material();

private:
	void bindMaterial(const glm::mat4& modelMat);
	void unbindMaterial();

	void setScenePointers(SceneLightManager* pSceneLightManager, SceneCamera* pSceneCamera, const glm::mat4& projMat);

	// ID of shader the material will use
	std::string m_shaderID;

	// Shader object the material will use - set once at material creation
	Shader* m_materialShader;

	std::string m_textureMapIDs[5];
	bool		m_textureMapUsing[5] = { false,false,false,false,false };

	bool m_normalMapNormalize;
	float m_heightMapHeight;

	SceneLightManager* m_pSceneLightManager;
	SceneCamera* m_pSceneCamera;
	const glm::mat4* m_pAppProjectionMatrix;

	// Ensures only the MaterialManager can call functions of a material object
	friend class MaterialManager;
};
