#pragma once

// Everything that uses MaterialManager will use Material
#include "Rendering/Material.h"

typedef std::unordered_map<std::string, std::shared_ptr<Material>> MaterialPool;


/// <summary>
/// Encapsulates and abstracts the creation, deletion and usage of material objects
/// Materials can only be used through this class
/// </summary>
class MaterialManager
{
public:
	bool			CreateMaterial(const std::string& materialID, const MaterialLoaderParams& pParams);

	void			BindMaterialAtID(const std::string& materialID, const glm::mat4& modelMat);
	void			UnbindMaterialAtID(const std::string& materialID);

	void			ClearAllMaterials();

	void			SetAllMaterialScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera);

	std::weak_ptr<Material> GetMaterialAtID(const std::string& materialID)
	{
		return m_materialMap.count(materialID) ? m_materialMap.at(materialID) : nullptr;
	}

	static MaterialManager* Instance()
	{
		static MaterialManager* s_pInstance = new MaterialManager;
		return s_pInstance;
	}
private:

	MaterialPool	m_materialMap;

	MaterialManager() {}
	~MaterialManager() {}
	MaterialManager(const MaterialManager&) = delete;
};
typedef MaterialManager TheMaterialManager;
