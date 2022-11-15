#pragma once

// Everything that uses MaterialManager will use Material
#include "Rendering/Material.h"

class MaterialManager
{
public:

	bool createMaterial(const std::string materialID, const MaterialLoaderParams& pParams);

	Material* getMaterialAtID(const std::string materialID);

	void clearAllMaterials();


	static MaterialManager* Instance()
	{
		static MaterialManager* s_pInstance = new MaterialManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, Material*> m_materialMap;


	MaterialManager() {}
	~MaterialManager() {}

};
typedef MaterialManager TheMaterialManager;