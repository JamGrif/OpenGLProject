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

SceneParser::SceneParser()
{
}

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
	TiXmlElement* pRoot = levelDocument.RootElement();

	// Create sky using skyid as the cubemap filename
	*sceneSky = std::make_shared<SceneSky>(pRoot->Attribute("skyid"));
	m_tempSkyCubemapID = pRoot->Attribute("skyid");

	TiXmlElement* materialRoot = nullptr;
	TiXmlElement* lightRoot = nullptr;
	TiXmlElement* modelRoot = nullptr;

	// Loop through all <scene> elements
	for (TiXmlElement* e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		// <materials> node
		if (strcmp(e->Value(), "materials") == STRCMP_SUCCESS)
		{
			materialRoot = e;
			continue;
		}

		// <lights> node
		if (strcmp(e->Value(), "lights") == STRCMP_SUCCESS)
		{
			lightRoot = e;
			continue;
		}

		// <models> node
		if (strcmp(e->Value(), "models") == STRCMP_SUCCESS)
		{
			modelRoot = e;
			continue;
		}
	}

	const auto processor_count = std::thread::hardware_concurrency();
	PRINT_TRACE("number of threads {0}", processor_count);

	// Parse all the assets used in scene
	PerformanceTimer parseTimer("Asset Parsing");

	ParseMaterialsNode(materialRoot);

	// Load all textures and meshes concurrently
	std::thread	firstMaterialThread(&SceneParser::ParseFirstHalfMaterials, this);
	std::thread secondMaterialThread(&SceneParser::ParseSecondHalfMaterials, this);
	std::thread modelThread(&SceneParser::ParseModelsNode, this, std::ref(modelRoot), std::ref(sceneModels));

	firstMaterialThread.join();
	secondMaterialThread.join();
	modelThread.join();

	// Add scene lights to the light manager
	ParseLightsNode(lightRoot, sceneLightManager);

	// Add the cubemap that the Skybox will use
	CubemapManager::Get()->AddResource(m_tempSkyCubemapID);

	parseTimer.stop();

	// Create all assets used in scene
	PerformanceTimer creationTimer("Asset Creation");
	ShaderManager::Get()->CreateAllResources();
	TextureManager::Get()->CreateAllResources();
	MeshManager::Get()->CreateAllResources();
	CubemapManager::Get()->CreateAllResources();

	creationTimer.stop();

	return true;
}

/// <summary>
/// Parse the <materials> node
/// </summary>
void SceneParser::ParseMaterialsNode(const TiXmlElement* pMaterialsRoot)
{
	bool bInsertInFirst = true;

	// Loop through all elements of <materials> node
	for (const TiXmlElement* materialNode = pMaterialsRoot->FirstChildElement(); materialNode != NULL; materialNode = materialNode->NextSiblingElement())
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
void SceneParser::ParseLightsNode(const TiXmlElement* pLightsRoot, std::shared_ptr<SceneLightManager>& sceneLightManager)
{
	// Loop through all elements of <lights> node
	for (const TiXmlElement* lightNode = pLightsRoot->FirstChildElement(); lightNode != NULL; lightNode = lightNode->NextSiblingElement())
	{
		// Ensure element is a <light>
		if (strcmp(lightNode->Value(), "light") != STRCMP_SUCCESS)
			continue;
		
		// Create light depending on what type it is
		std::string_view type = lightNode->Attribute("type");
		if (type == "direction")
		{
			DirectionalLoaderParams* tempParams = new DirectionalLoaderParams;

			ParseBaseLight(lightNode, tempParams);

			float a, b, c;

			lightNode->QueryFloatAttribute("dirX", &a);
			lightNode->QueryFloatAttribute("dirY", &b);
			lightNode->QueryFloatAttribute("dirZ", &c);

			Vector3D temp{ a,b,c };

			tempParams->direction = temp;

			sceneLightManager->AddDirectionalLight(tempParams);
		}
		else if (type == "point")
		{
			PointLoaderParams* tempParams = new PointLoaderParams;

			ParseBaseLight(lightNode, tempParams);

			float a, b, c;

			lightNode->QueryFloatAttribute("posX", &a);
			lightNode->QueryFloatAttribute("posY", &b);
			lightNode->QueryFloatAttribute("posZ", &c);

			Vector3D temp{ a,b,c };

			tempParams->position = temp;
			
			sceneLightManager->AddPointLight(tempParams);
		}
		else if (type == "spot")
		{
			SpotLoaderParams* tempParams = new SpotLoaderParams;

			ParseBaseLight(lightNode, tempParams);

			float a, b, c, d, e, f;

			lightNode->QueryFloatAttribute("posX", &a);
			lightNode->QueryFloatAttribute("posY", &b);
			lightNode->QueryFloatAttribute("posZ", &c);

			lightNode->QueryFloatAttribute("dirX", &d);
			lightNode->QueryFloatAttribute("dirY", &e);
			lightNode->QueryFloatAttribute("dirZ", &f);

			Vector3D temp1{ a,b,c };
			Vector3D temp2{ d,e,f };

			tempParams->position = temp1;
			tempParams->direction = temp2;

			sceneLightManager->AddSpotLight(tempParams);
		}
	}
}

/// <summary>
/// Parse the <models> node
/// </summary>
void SceneParser::ParseModelsNode(const TiXmlElement* pModelRoot, SceneModels& sceneModels)
{
	// Loop through all the elements of <models> node
	for (const TiXmlElement* modelNode = pModelRoot->FirstChildElement(); modelNode != NULL; modelNode = modelNode->NextSiblingElement())
	{
		// Ensure element is a <model>
		if (strcmp(modelNode->Value(), "model") != STRCMP_SUCCESS)
			continue;

		// Fill out initial value of a model from XML scene data
		ModelLoaderParams tempLoaderParams;

		modelNode->QueryStringAttribute("material", &tempLoaderParams.materialID);
		modelNode->QueryStringAttribute("mesh",		&tempLoaderParams.meshID);

		modelNode->QueryFloatAttribute("posX",		&tempLoaderParams.posX);
		modelNode->QueryFloatAttribute("posY",		&tempLoaderParams.posY);
		modelNode->QueryFloatAttribute("posZ",		&tempLoaderParams.posZ);
		
		modelNode->QueryFloatAttribute("rotX",		&tempLoaderParams.rotX);
		modelNode->QueryFloatAttribute("rotY",		&tempLoaderParams.rotY);
		modelNode->QueryFloatAttribute("rotZ",		&tempLoaderParams.rotZ);

		modelNode->QueryFloatAttribute("scaleX",	&tempLoaderParams.scaleX);
		modelNode->QueryFloatAttribute("scaleY",	&tempLoaderParams.scaleY);
		modelNode->QueryFloatAttribute("scaleZ",	&tempLoaderParams.scaleZ);
		
		sceneModels.emplace_back(std::make_unique<Model>(tempLoaderParams));

		// Use the meshID to create initial mesh
		MeshManager::Get()->AddResource(tempLoaderParams.meshID);
	}
}

/// <summary>
/// Fill out the base light loading parameters - all lights share these common values
/// </summary>
void SceneParser::ParseBaseLight(const TiXmlElement* lightNode, LightLoaderParams* pParams)
{

	float a, b, c, d, e, f, g, h, i;
	
	lightNode->QueryFloatAttribute("ambR", &a);
	lightNode->QueryFloatAttribute("ambG", &b);
	lightNode->QueryFloatAttribute("ambB", &c);

	lightNode->QueryFloatAttribute("difR", &d);
	lightNode->QueryFloatAttribute("difG", &e);
	lightNode->QueryFloatAttribute("difB", &f);

	lightNode->QueryFloatAttribute("speR", &g);
	lightNode->QueryFloatAttribute("speG", &h);
	lightNode->QueryFloatAttribute("speB", &i);


	Vector3D temp1{ a,b,c };
	Vector3D temp2{ d,e,f };
	Vector3D temp3{ g,h,i };

	pParams->ambient = temp1;
	pParams->diffuse = temp2;
	pParams->specular = temp3;

}
