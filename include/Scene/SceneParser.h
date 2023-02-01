#pragma once

class TiXmlElement;

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
	void ParseMaterialsNode(const TiXmlElement* pMaterialsElement);
	void ParseModelsNode(const TiXmlElement* pModelElement, SceneModels& sceneModels);
	void ParseLightsNode(const TiXmlElement* pLightsElement, std::shared_ptr<SceneLightManager>& sceneLightManager);

	void ParseFirstHalfMaterials();
	void ParseSecondHalfMaterials();

	void ParseBaseLight(const TiXmlElement* lightNode, std::shared_ptr<LightLoaderParams> pParams);

	PendingMaterialPool m_firstMatMap;
	PendingMaterialPool m_secondMatMap;

	ResourceID m_tempSkyCubemapID;
};
