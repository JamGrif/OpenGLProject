#pragma once

class Model;
class SceneLightManager;
class SceneSky;

class TiXmlElement;

struct LightLoaderParams;

/// <summary>
/// This class reads in a textfile containing all of the scene information from res/scenes/
/// 
/// The textfile contains every entity/light in the scene along with all the data it uses (position data, textures, colours etc...)
/// 
/// It then uses the text information to fill out a "template", unique to every type of entity/light.
/// 
/// This template contains all the information a entity/light needs to be correctly created and rendered into the world
/// 
/// The template is then passed to the newly created entity/light, through its constructor where the template information is used to set the values,
/// and then added to the scene vectors
/// </summary>
class SceneParser
{
public:
	SceneParser(const std::string& filename, std::vector<std::shared_ptr<Model>>& sceneLightingEntities, std::shared_ptr<SceneLightManager>& sceneLightManager, SceneSky** sceneSky);
	~SceneParser() {};

	bool getStatus() { return m_status; }

private:
	void parseMaterials(TiXmlElement* pMaterialsRoot);
	void parseLights(TiXmlElement* pLightsRoot, std::shared_ptr<SceneLightManager>& sceneLightManager);
	void parseModels(TiXmlElement* pModelRoot, std::vector<std::shared_ptr<Model>>& sceneLightingEntities);

	void parseBaseLight(TiXmlElement* e, LightLoaderParams* pParams);

	bool m_status;

};
