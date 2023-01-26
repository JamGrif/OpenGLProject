#pragma once

class TiXmlElement;

class SceneLightManager;
class SceneSky;

class Model;
struct LightLoaderParams;
struct MaterialLoaderParams;

// Material data that is waiting to be parsed and created
typedef std::unordered_map<std::string, MaterialLoaderParams> PendingMaterialPool;
typedef std::vector<std::unique_ptr<Model>> SceneModels;

/// <summary>
/// Parser reads in a XML file containing all of the scene information from res/scenes/
/// Parses the scene XML file and creates all objects, assets and light data the scene will use
/// </summary>
class SceneParser
{
public:
	SceneParser();
	~SceneParser() {};

	bool ParseSceneFile(const std::string& sceneFilepath, SceneModels& sceneModels, std::shared_ptr<SceneLightManager>& sceneLightManager, std::shared_ptr<SceneSky>* sceneSky);

private:
	void ParseMaterialsNode(const TiXmlElement* pMaterialsRoot);
	void ParseModelsNode(const TiXmlElement* pModelRoot, SceneModels& sceneModels);
	void ParseLightsNode(const TiXmlElement* pLightsRoot, std::shared_ptr<SceneLightManager>& sceneLightManager);

	void ParseFirstHalfMaterials();
	void ParseSecondHalfMaterials();

	void ParseBaseLight(const TiXmlElement* lightNode, LightLoaderParams* pParams);

	PendingMaterialPool m_firstMatMap;
	PendingMaterialPool m_secondMatMap;

	std::string m_tempSkyCubemapID;
};
