#include "pch.h"
#include "Scene/SceneParser.h"

#include <thread>

#include "tinyXML/tinyxml.h"	// Read the level data

#include "Scene/SceneSky.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Resource/Manager/ShaderManager.h"
#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/Manager/TextureManager.h"
#include "Rendering/Resource/Manager/MeshManager.h"
#include "Rendering/Resource/Manager/CubemapManager.h"
#include "Rendering/Model.h"

static constexpr uint8_t STRCMP_SUCCESS = 0;

SceneParser::SceneParser()
{
}

bool SceneParser::ParseSceneFile(const std::string& filename, std::vector<std::shared_ptr<Model>>& sceneLightingEntities, std::shared_ptr<SceneLightManager>& sceneLightManager, std::shared_ptr<SceneSky>* sceneSky)
{
	TiXmlDocument levelDocument;

	// Load scene XML file and check status
	if (!levelDocument.LoadFile(filename, TIXML_ENCODING_UTF8))
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
	std::thread modelThread(&SceneParser::ParseModelsNode, this, std::ref(modelRoot), std::ref(sceneLightingEntities));

	firstMaterialThread.join();
	secondMaterialThread.join();
	modelThread.join();

	// Add scene lights to the light manager
	ParseLightsNode(lightRoot, sceneLightManager);

	// Add the cubemap that the Skybox will use
	TheCubemapManager::Instance()->AddCubemap(m_tempSkyCubemapID);

	parseTimer.stop();

	// Create all assets used in scene
	PerformanceTimer creationTimer("Asset Creation");
	TheShaderManager::Instance()->CreateAllShaders();
	TheTextureManager::Instance()->CreateAllTextures();
	TheMeshManager::Instance()->CreateAllMeshes();
	TheCubemapManager::Instance()->CreateAllCubemaps();

	creationTimer.stop();

	return true;
}

/// <summary>
/// Parse the <materials> node
/// </summary>
void SceneParser::ParseMaterialsNode(TiXmlElement* pMaterialsRoot)
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

		materialNode->QueryStringAttribute("diffuseid", &tempLoaderParams.diffuseMapID);
		materialNode->QueryStringAttribute("specularid", &tempLoaderParams.specularMapID);
		materialNode->QueryStringAttribute("normalid", &tempLoaderParams.normalMapID);
		materialNode->QueryStringAttribute("heightid", &tempLoaderParams.heightMapID);
		materialNode->QueryStringAttribute("emissionid", &tempLoaderParams.emissionMapID);

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
void SceneParser::ParseLightsNode(TiXmlElement* pLightsRoot, std::shared_ptr<SceneLightManager>& sceneLightManager)
{
	// Loop through all elements of <lights> node
	for (TiXmlElement* lightNode = pLightsRoot->FirstChildElement(); lightNode != NULL; lightNode = lightNode->NextSiblingElement())
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
			
			lightNode->QueryFloatAttribute("dirX", &tempParams->direction.x);
			lightNode->QueryFloatAttribute("dirY", &tempParams->direction.y);
			lightNode->QueryFloatAttribute("dirZ", &tempParams->direction.z);

			sceneLightManager->AddDirectionalLight(tempParams);
		}
		else if (type == "point")
		{
			PointLoaderParams* tempParams = new PointLoaderParams;

			ParseBaseLight(lightNode, tempParams);

			lightNode->QueryFloatAttribute("posX", &tempParams->position.x);
			lightNode->QueryFloatAttribute("posY", &tempParams->position.y);
			lightNode->QueryFloatAttribute("posZ", &tempParams->position.z);
			
			sceneLightManager->AddPointLight(tempParams);
		}
		else if (type == "spot")
		{
			SpotLoaderParams* tempParams = new SpotLoaderParams;

			ParseBaseLight(lightNode, tempParams);

			lightNode->QueryFloatAttribute("posX", &tempParams->position.x);
			lightNode->QueryFloatAttribute("posY", &tempParams->position.y);
			lightNode->QueryFloatAttribute("posZ", &tempParams->position.z);

			lightNode->QueryFloatAttribute("dirX", &tempParams->direction.x);
			lightNode->QueryFloatAttribute("dirY", &tempParams->direction.y);
			lightNode->QueryFloatAttribute("dirZ", &tempParams->direction.z);

			sceneLightManager->AddSpotLight(tempParams);
		}
	}
}

/// <summary>
/// Parse the <models> node
/// </summary>
void SceneParser::ParseModelsNode(TiXmlElement* pModelRoot, std::vector<std::shared_ptr<Model>>& sceneLightingEntities)
{
	// Loop through all the elements of <models> node
	for (const TiXmlElement* modelNode = pModelRoot->FirstChildElement(); modelNode != NULL; modelNode = modelNode->NextSiblingElement())
	{
		// Ensure element is a <model>
		if (strcmp(modelNode->Value(), "model") != STRCMP_SUCCESS)
			continue;

		ModelLoaderParams tempLoaderParams;

		modelNode->QueryStringAttribute("material", &tempLoaderParams.materialID);
		modelNode->QueryStringAttribute("mesh", &tempLoaderParams.meshID);

		modelNode->QueryFloatAttribute("posX", &tempLoaderParams.posX);
		modelNode->QueryFloatAttribute("posY", &tempLoaderParams.posY);
		modelNode->QueryFloatAttribute("posZ", &tempLoaderParams.posZ);
		
		modelNode->QueryFloatAttribute("rotX", &tempLoaderParams.rotX);
		modelNode->QueryFloatAttribute("rotY", &tempLoaderParams.rotY);
		modelNode->QueryFloatAttribute("rotZ", &tempLoaderParams.rotZ);

		modelNode->QueryFloatAttribute("scaleX", &tempLoaderParams.scaleX);
		modelNode->QueryFloatAttribute("scaleY", &tempLoaderParams.scaleY);
		modelNode->QueryFloatAttribute("scaleZ", &tempLoaderParams.scaleZ);
		
		sceneLightingEntities.emplace_back(std::make_shared<Model>(tempLoaderParams));
	}
}

/// <summary>
/// Fill out the base light loading parameters - all lights share these common values
/// </summary>
void SceneParser::ParseBaseLight(TiXmlElement* lightNode, LightLoaderParams* pParams)
{
	lightNode->QueryFloatAttribute("ambR", &pParams->ambient.r);
	lightNode->QueryFloatAttribute("ambG", &pParams->ambient.g);
	lightNode->QueryFloatAttribute("ambB", &pParams->ambient.b);

	lightNode->QueryFloatAttribute("difR", &pParams->diffuse.r);
	lightNode->QueryFloatAttribute("difG", &pParams->diffuse.g);
	lightNode->QueryFloatAttribute("difB", &pParams->diffuse.b);

	lightNode->QueryFloatAttribute("speR", &pParams->specular.r);
	lightNode->QueryFloatAttribute("speG", &pParams->specular.g);
	lightNode->QueryFloatAttribute("speB", &pParams->specular.b);
}
