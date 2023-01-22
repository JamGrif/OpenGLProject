#pragma once

#include "glm/mat4x4.hpp"

class Shader;
class SceneCamera;
class SceneLightManager;

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

/// <summary>
/// Encapsulates multiple Texture objects and a single Shader object
/// Used in the Model class with a Mesh to render
/// </summary>
class Material
{
public:
	Material(const MaterialLoaderParams& pParams);
	~Material();

private:

	void		BindMaterial(const glm::mat4& modelMat);
	void		UnbindMaterial();

	void		SetScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera);

private:

	// Shader the material uses
	std::string m_shaderID;

	// Textures the material uses
	std::string m_textureMapIDs[5];
	bool		m_textureMapUsing[5] = { false,false,false,false,false };

	// Rendering data
	bool		m_bNormalMapNormalize;
	float		m_heightMapHeight;

	// Cached scene objects
	std::weak_ptr<SceneLightManager> m_pSceneLightManager;
	std::weak_ptr<SceneCamera> m_pSceneCamera;
	const glm::mat4& m_pAppProjectionMatrix;

	// Ensures only the MaterialManager can call functions of a material object
	friend class MaterialManager;
};
