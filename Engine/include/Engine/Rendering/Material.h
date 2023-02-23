#pragma once

#include "glm/mat4x4.hpp"

/// <summary>
/// Material loading parameters which are passed to a Material object on construction to set initial values
/// </summary>
struct MaterialLoaderParams
{
	std::string textureMapIDs[MATERIAL_TEXTURE_SLOTS];

	// Should normal map be normalized in fragment shader?
	bool		normalMapNormalize;

	// Height to render the height map within fragment shader
	float		heightMapHeight;	
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

	/// <summary>
	/// Return IDs of all textures the materials uses
	/// </summary>
	const MaterialTextures& GetAllTextureIDs()
	{
		return m_textureMapIDs;
	}

private:

	void			BindMaterial(const glm::mat4& modelMat);
	void			UnbindMaterial();

	void			SetScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera);

private:

	// Shader the material uses
	std::string		m_shaderID;

	// Textures the material uses
	typedef std::array<bool, MATERIAL_TEXTURE_SLOTS> MaterialUsingTextures;
	MaterialUsingTextures m_textureMapUsing;

	MaterialTextures m_textureMapIDs;

	// Rendering data
	bool			m_bNormalMapNormalize;
	float			m_heightMapHeight;

	// Cached scene objects
	std::weak_ptr<SceneLightManager> m_pSceneLightManager;
	std::weak_ptr<SceneCamera> m_pSceneCamera;
	const glm::mat4& m_pAppProjectionMatrix;

	// Ensures only the MaterialManager can call functions of a material object
	friend class MaterialManager;
};
