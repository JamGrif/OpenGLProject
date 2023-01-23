#pragma once

class TiXmlElement;

class SceneLightManager;
class SceneSky;

class Model;
struct LightLoaderParams;
struct MaterialLoaderParams;

// Holds material data that is waiting to be parsed and created
typedef std::unordered_map<std::string, MaterialLoaderParams> PendingMaterialPool;

/// <summary>
/// This class reads in a XML file containing all of the scene information from res/scenes/
/// Parses the scene XML file and creates all objects, assets and light data the scene will use
/// </summary>
class SceneParser
{
public:
	SceneParser();
	~SceneParser() {};

	bool ParseSceneFile(const std::string& filename, std::vector<std::shared_ptr<Model>>& sceneLightingEntities, std::shared_ptr<SceneLightManager>& sceneLightManager, std::shared_ptr<SceneSky>* sceneSky);

private:
	void ParseMaterialsNode(TiXmlElement* pMaterialsRoot);
	void ParseModelsNode(TiXmlElement* pModelRoot, std::vector<std::shared_ptr<Model>>& sceneLightingEntities);
	void ParseLightsNode(TiXmlElement* pLightsRoot, std::shared_ptr<SceneLightManager>& sceneLightManager);

	void ParseFirstHalfMaterials();
	void ParseSecondHalfMaterials();

	void ParseBaseLight(TiXmlElement* lightNode, LightLoaderParams* pParams);

	PendingMaterialPool m_firstMatMap;
	PendingMaterialPool m_secondMatMap;

	std::string m_tempSkyCubemapID;
};
