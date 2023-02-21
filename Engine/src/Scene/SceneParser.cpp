#include "pch.h"
#include "Scene/SceneParser.h"

#include <thread>

#include "tinyXML/tinyxml.h"	// Read the level data

#include "Scene/SceneSky.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Rendering/Model.h"

static constexpr uint8_t STRCMP_SUCCESS = 0;

static const std::string SCENE_FILEPATH_PREFIX = "res/scenes/";
static const std::string SCENE_FILEPATH_SUFFIX = ".xml";

static const char* SKY_ATTRIBUTE = "skyid";

static const char* MATERIALS_ELEMENT = "materials";
static const char* LIGHTS_ELEMENT = "lights";
static const char* MODELS_ELEMENT = "models";

SceneParser::SceneParser()
{
}

/// <summary>
/// Parse the .xml scene file at sceneFilepath and fill the SceneModels vector, create all lights through the SceneLightManager and create the sky cubemap at sceneSky
/// </summary>
bool SceneParser::ParseSceneFile(const std::string& sceneFilepath, SceneModels& sceneModels, std::shared_ptr<SceneLightManager>& sceneLightManager, std::shared_ptr<SceneSky>* sceneSky)
{
	TiXmlDocument levelDocument;

	// Load scene XML file and check status
	if (!levelDocument.LoadFile(SCENE_FILEPATH_PREFIX + sceneFilepath + SCENE_FILEPATH_SUFFIX, TIXML_ENCODING_UTF8))
	{
		PRINT_TRACE("{0}", levelDocument.ErrorDesc());
		return false;
	}

	// Store <scene> node
	TiXmlElement* pFileRoot = levelDocument.RootElement();

	// Create sky using skyid as the cubemap filename
	*sceneSky = std::make_shared<SceneSky>(pFileRoot->Attribute(SKY_ATTRIBUTE));
	ResourceID m_tempSkyCubemapID = pFileRoot->Attribute(SKY_ATTRIBUTE);

	const TiXmlElement* pMaterialElement = nullptr, * pLightElement = nullptr, * pModelElement = nullptr;

	// Loop through all <scene> elements
	for (TiXmlElement* currentElement = pFileRoot->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
	{
		// <materials> element
		if (strcmp(currentElement->Value(), MATERIALS_ELEMENT) == STRCMP_SUCCESS)
		{
			pMaterialElement = currentElement;
			continue;
		}

		// <lights> element
		if (strcmp(currentElement->Value(), LIGHTS_ELEMENT) == STRCMP_SUCCESS)
		{
			pLightElement = currentElement;
			continue;
		}

		// <models> element
		if (strcmp(currentElement->Value(), MODELS_ELEMENT) == STRCMP_SUCCESS)
		{
			pModelElement = currentElement;
			continue;
		}
	}

	//PRINT_TRACE("number of threads {0}", std::thread::hardware_concurrency());

	// Parse all the assets used in scene
	PerformanceTimer parseTimer("Asset Parsing");

	ParseMaterialsNode(pMaterialElement);

	// Load all textures and meshes concurrently
	std::thread	firstMaterialThread(&SceneParser::ParseFirstHalfMaterials, this);
	std::thread secondMaterialThread(&SceneParser::ParseSecondHalfMaterials, this);
	std::thread modelThread(&SceneParser::ParseModelsNode, this, std::ref(pModelElement), std::ref(sceneModels));

	firstMaterialThread.join();
	secondMaterialThread.join();
	modelThread.join();

	// Add scene lights to the light manager
	ParseLightsNode(pLightElement, sceneLightManager);

	// Add the cubemap that the Skybox will use
	CubemapManager::Get()->AddResource(m_tempSkyCubemapID);

	parseTimer.Stop();

	// Create all assets used in scene
	PerformanceTimer creationTimer("Asset Creation");

	ShaderManager::Get()->CreateAllResources();
	TextureManager::Get()->CreateAllResources();
	MeshManager::Get()->CreateAllResources();
	CubemapManager::Get()->CreateAllResources();

	creationTimer.Stop();

	return true;
}

/// <summary>
/// Parse the <materials> node
/// </summary>
void SceneParser::ParseMaterialsNode(const TiXmlElement* pMaterialsElement)
{
	bool bInsertInFirst = true;

	// Loop through all elements of <materials> node
	for (const TiXmlElement* materialNode = pMaterialsElement->FirstChildElement(); materialNode != NULL; materialNode = materialNode->NextSiblingElement())
	{
		// Ensure element is a <material> element
		if (strcmp(materialNode->Value(), "material") != STRCMP_SUCCESS)
			continue;

		// Fill out materials loading parameters
		MaterialLoaderParams tempLoaderParams;

		materialNode->QueryStringAttribute("diffuseid", &tempLoaderParams.textureMapIDs[static_cast<int>(TextureType::DIFFUSE)]);
		materialNode->QueryStringAttribute("specularid", &tempLoaderParams.textureMapIDs[static_cast<int>(TextureType::SPECULAR)]);
		materialNode->QueryStringAttribute("normalid", &tempLoaderParams.textureMapIDs[static_cast<int>(TextureType::NORMAL)]);
		materialNode->QueryStringAttribute("heightid", &tempLoaderParams.textureMapIDs[static_cast<int>(TextureType::HEIGHT)]);
		materialNode->QueryStringAttribute("emissionid", &tempLoaderParams.textureMapIDs[static_cast<int>(TextureType::EMISSION)]);

		materialNode->QueryBoolAttribute("normalmapNormalize", &tempLoaderParams.normalMapNormalize);
		materialNode->QueryFloatAttribute("heightmapHeight", &tempLoaderParams.heightMapHeight);

		// Materials are split into separate containers to allow faster parsing through multi threading
		if (bInsertInFirst)
		{
			bInsertInFirst = !bInsertInFirst;
			m_firstMatMap.insert({ materialNode->Attribute("id"), tempLoaderParams });
		}
		else
		{
			bInsertInFirst = !bInsertInFirst;
			m_secondMatMap.insert({ materialNode->Attribute("id"), tempLoaderParams });
		}
	}
}

/// <summary>
/// Parse one half of the materials used in the scene
/// </summary>
void SceneParser::ParseFirstHalfMaterials()
{
	// Create material using filled out loading parameters and assign the id to it
	for (const auto& [materialID, materialLoaderParams] : m_firstMatMap)
	{
		TheMaterialManager::Instance()->CreateMaterial(materialID, materialLoaderParams);
	}
}

/// <summary>
/// Parse the other half of the materials used in the scene
/// </summary>
void SceneParser::ParseSecondHalfMaterials()
{
	// Create material using filled out loading parameters and assign the id to it
	for (const auto& [materialID, materialLoaderParams] : m_secondMatMap)
	{
		TheMaterialManager::Instance()->CreateMaterial(materialID, materialLoaderParams);
	}
}

/// <summary>
/// Parse the <lights> node
/// </summary>
void SceneParser::ParseLightsNode(const TiXmlElement* pLightsElement, std::shared_ptr<SceneLightManager>& sceneLightManager)
{
	// Loop through all elements of <lights> node
	for (const TiXmlElement* lightElement = pLightsElement->FirstChildElement(); lightElement != NULL; lightElement = lightElement->NextSiblingElement())
	{
		// Ensure element is a <light>
		if (strcmp(lightElement->Value(), "light") != STRCMP_SUCCESS)
			continue;
		
		// Create light depending on what type it is
		std::string_view type = lightElement->Attribute("type");
		if (type == "direction")
		{
			std::shared_ptr<DirectionalLoaderParams> tempParams = std::make_shared<DirectionalLoaderParams>();

			ParseBaseLight(lightElement, tempParams);

			lightElement->QueryFloatAttribute("dirX", &tempParams->direction.SetX());
			lightElement->QueryFloatAttribute("dirY", &tempParams->direction.SetY());
			lightElement->QueryFloatAttribute("dirZ", &tempParams->direction.SetZ());

			sceneLightManager->AddDirectionalLight(tempParams);
		}
		else if (type == "point")
		{
			std::shared_ptr<PointLoaderParams> tempParams = std::make_shared<PointLoaderParams>();

			ParseBaseLight(lightElement, tempParams);

			lightElement->QueryFloatAttribute("posX", &tempParams->position.SetX());
			lightElement->QueryFloatAttribute("posY", &tempParams->position.SetY());
			lightElement->QueryFloatAttribute("posZ", &tempParams->position.SetZ());

			sceneLightManager->AddPointLight(tempParams);
		}
		else if (type == "spot")
		{
			std::shared_ptr<SpotLoaderParams> tempParams = std::make_shared<SpotLoaderParams>();

			ParseBaseLight(lightElement, tempParams);

			lightElement->QueryFloatAttribute("posX", &tempParams->position.SetX());
			lightElement->QueryFloatAttribute("posY", &tempParams->position.SetY());
			lightElement->QueryFloatAttribute("posZ", &tempParams->position.SetZ());

			lightElement->QueryFloatAttribute("dirX", &tempParams->direction.SetX());
			lightElement->QueryFloatAttribute("dirY", &tempParams->direction.SetY());
			lightElement->QueryFloatAttribute("dirZ", &tempParams->direction.SetZ());

			sceneLightManager->AddSpotLight(tempParams);
		}
	}
}

/// <summary>
/// Parse the <models> node
/// </summary>
void SceneParser::ParseModelsNode(const TiXmlElement* pModelElement, SceneModels& sceneModels)
{
	std::unordered_multimap<std::string, std::string> tempMap;

	// Loop through all the elements of <models> node
	for (const TiXmlElement* modelElement = pModelElement->FirstChildElement(); modelElement != NULL; modelElement = modelElement->NextSiblingElement())
	{
		// Ensure element is a <model>
		if (strcmp(modelElement->Value(), "model") != STRCMP_SUCCESS)
			continue;

		// Fill out initial value of a model from XML scene data
		ModelLoaderParams tempLoaderParams;

		modelElement->QueryStringAttribute("material",	&tempLoaderParams.materialID);
		modelElement->QueryStringAttribute("mesh",		&tempLoaderParams.meshID);

		modelElement->QueryFloatAttribute("posX",		&tempLoaderParams.position.SetX());
		modelElement->QueryFloatAttribute("posY",		&tempLoaderParams.position.SetY());
		modelElement->QueryFloatAttribute("posZ",		&tempLoaderParams.position.SetZ());
		
		modelElement->QueryFloatAttribute("rotX",		&tempLoaderParams.rotation.SetX());
		modelElement->QueryFloatAttribute("rotY",		&tempLoaderParams.rotation.SetY());
		modelElement->QueryFloatAttribute("rotZ",		&tempLoaderParams.rotation.SetZ());

		modelElement->QueryFloatAttribute("scaleX",		&tempLoaderParams.scale.SetX());
		modelElement->QueryFloatAttribute("scaleY",		&tempLoaderParams.scale.SetY());
		modelElement->QueryFloatAttribute("scaleZ",		&tempLoaderParams.scale.SetZ());

		// Set models ID as number of times the mesh has currently been used in the scene
		tempMap.insert({ tempLoaderParams.meshID , tempLoaderParams.meshID });
		tempLoaderParams.modelID = tempLoaderParams.meshID + " " + std::to_string(tempMap.count(tempLoaderParams.meshID));

		sceneModels.emplace_back(std::make_shared<Model>(tempLoaderParams));

		// Use the meshID to create initial mesh
		MeshManager::Get()->AddResource(tempLoaderParams.meshID);
	}
}

/// <summary>
/// Fill out the base light loading parameters - all lights share these common values
/// </summary>
void SceneParser::ParseBaseLight(const TiXmlElement* lightNode, std::shared_ptr<ILightLoaderParams> pParams)
{
	lightNode->QueryFloatAttribute("ambR", &pParams->ambient.SetX());
	lightNode->QueryFloatAttribute("ambG", &pParams->ambient.SetY());
	lightNode->QueryFloatAttribute("ambB", &pParams->ambient.SetZ());

	lightNode->QueryFloatAttribute("difR", &pParams->diffuse.SetX());
	lightNode->QueryFloatAttribute("difG", &pParams->diffuse.SetY());
	lightNode->QueryFloatAttribute("difB", &pParams->diffuse.SetZ());

	lightNode->QueryFloatAttribute("speR", &pParams->specular.SetX());
	lightNode->QueryFloatAttribute("speG", &pParams->specular.SetY());
	lightNode->QueryFloatAttribute("speB", &pParams->specular.SetZ());
}
