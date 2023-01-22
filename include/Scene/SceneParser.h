#pragma once

class Model;
class SceneLightManager;
class SceneSky;

class TiXmlElement;

struct LightLoaderParams;
struct MaterialLoaderParams;

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
	void ParseMaterials(TiXmlElement* pMaterialsRoot);

	void ParseFirstHalfMaterials();
	void ParseSecondHalfMaterials();

	std::unordered_map<std::string, MaterialLoaderParams> firstMatMap;
	std::unordered_map<std::string, MaterialLoaderParams> secondMatMap;

	void ParseLights(TiXmlElement* pLightsRoot, std::shared_ptr<SceneLightManager>& sceneLightManager);
	void ParseModels(TiXmlElement* pModelRoot, std::vector<std::shared_ptr<Model>>& sceneLightingEntities);

	void ParseBaseLight(TiXmlElement* lightNode, LightLoaderParams* pParams);

	std::string m_tempSkyCubemapID;
};
