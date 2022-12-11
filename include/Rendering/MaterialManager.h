#pragma once

// Everything that uses MaterialManager will use Material
#include "Rendering/Material.h"

/// <summary>
/// Abstracts and handles the creation, deletion and usage of material objects
/// Materials can only be used through this class
/// </summary>
class MaterialManager
{
public:

	bool createMaterial(const std::string& materialID, const MaterialLoaderParams& pParams);

	void setAllMaterialPointers(SceneLightManager* pSceneLightManager, SceneCamera* pSceneCamera, const glm::mat4& projMat);

	void bindMaterialAtID(const std::string& materialID, const glm::mat4& modelMat);
	void unbindMaterialAtID(const std::string& materialID);

	void clearAllMaterials();

	static MaterialManager* Instance()
	{
		static MaterialManager* s_pInstance = new MaterialManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, std::unique_ptr<Material>> m_materialMap;

	MaterialManager() {}
	~MaterialManager() {}
	MaterialManager(const MaterialManager&) = delete;
};
typedef MaterialManager TheMaterialManager;
