#include "pch.h"
#include "Scene/SceneParser.h"

#include <thread>

#include "tinyXML/tinyxml.h"	// Read the level data

#include "Scene/SceneSky.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/ShaderManager.h"
#include "Rendering/MaterialManager.h"
#include "Rendering/TextureManager.h"
#include "Rendering/MeshManager.h"
#include "Rendering/Model.h"


static constexpr uint8_t STRCMP_SUCCESS = 0;

SceneParser::SceneParser(const std::string& filename, std::vector<std::shared_ptr<Model>>& sceneLightingEntities, std::shared_ptr<SceneLightManager>& sceneLightManager, SceneSky** sceneSky)
	:m_status(false)
{
	// Create the TinyXML document and load the map XML
	TiXmlDocument levelDocument;

	// Load scene file and check status
	if (!levelDocument.LoadFile(filename, TIXML_ENCODING_UTF8))
	{
		PRINT_TRACE("{0}", levelDocument.ErrorDesc());
		m_status = false;
		return;
	}

	// Store <scene> node
	TiXmlElement* pRoot = levelDocument.RootElement();

	// Create sky using skyid 
	*sceneSky = new SceneSky(pRoot->Attribute("skyid"));

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
			continue;;
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

	// Parse all the assets used in scene
	PerformanceTimer parseTimer("Asset Parsing");
	std::thread materialThread(&SceneParser::parseMaterials, this, materialRoot);
	std::thread modelThread(&SceneParser::parseModels, this, std::ref(modelRoot), std::ref(sceneLightingEntities));

	materialThread.join();
	modelThread.join();

	parseLights(lightRoot, sceneLightManager);

	parseTimer.stop();

	// Create all assets used in scene
	PerformanceTimer creationTimer("Asset creating");
	TheShaderManager::Instance()->createAllShaders();
	TheTextureManager::Instance()->createAllTextures();
	TheMeshManager::Instance()->createAllMeshes();

	creationTimer.stop();

	m_status = true;
}

/// <summary>
/// Parse the <materials> node
/// </summary>
void SceneParser::parseMaterials(TiXmlElement* pMaterialsRoot)
{
	// Loop through all elements of <materials> node
	for (const TiXmlElement* e = pMaterialsRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		// Ensure element is a <material> element
		if (strcmp(e->Value(), "material") != STRCMP_SUCCESS)
			continue;

		// Fill out materials loading parameters
		MaterialLoaderParams tempLoaderParams;

		e->QueryStringAttribute("diffuseid", &tempLoaderParams.diffuseMapID);
		e->QueryStringAttribute("specularid", &tempLoaderParams.specularMapID);
		e->QueryStringAttribute("normalid", &tempLoaderParams.normalMapID);
		e->QueryStringAttribute("heightid", &tempLoaderParams.heightMapID);
		e->QueryStringAttribute("emissionid", &tempLoaderParams.emissionMapID);

		e->QueryBoolAttribute("normalmapNormalize", &tempLoaderParams.normalMapNormalize);
		e->QueryFloatAttribute("heightmapHeight", &tempLoaderParams.heightMapHeight);

		// Create material using filled out loading parameters and assign the id to it
		TheMaterialManager::Instance()->addMaterial(e->Attribute("id"), tempLoaderParams);
	}
}

/// <summary>
/// Parse the <lights> node
/// </summary>
void SceneParser::parseLights(TiXmlElement* pLightsRoot, std::shared_ptr<SceneLightManager>& sceneLightManager)
{
	// Loop through all elements of <lights> node
	for (TiXmlElement* e = pLightsRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		// Ensure element is a <light>
		if (strcmp(e->Value(), "light") != STRCMP_SUCCESS)
			continue;
		
		// Create light depending on what type it is
		std::string_view type = e->Attribute("type");
		if (type == "direction")
		{
			DirectionalLoaderParams* tempParams = new DirectionalLoaderParams;

			parseBaseLight(e, tempParams);
			
			e->QueryFloatAttribute("dirX", &tempParams->direction.x);
			e->QueryFloatAttribute("dirY", &tempParams->direction.y);
			e->QueryFloatAttribute("dirZ", &tempParams->direction.z);

			sceneLightManager->addDirectionalLight(tempParams);
		}
		else if (type == "point")
		{
			PointLoaderParams* tempParams = new PointLoaderParams;

			parseBaseLight(e, tempParams);

			e->QueryFloatAttribute("posX", &tempParams->position.x);
			e->QueryFloatAttribute("posY", &tempParams->position.y);
			e->QueryFloatAttribute("posZ", &tempParams->position.z);
			
			sceneLightManager->addPointLight(tempParams);
		}
		else if (type == "spot")
		{
			SpotLoaderParams* tempParams = new SpotLoaderParams;

			parseBaseLight(e, tempParams);

			e->QueryFloatAttribute("posX", &tempParams->position.x);
			e->QueryFloatAttribute("posY", &tempParams->position.y);
			e->QueryFloatAttribute("posZ", &tempParams->position.z);

			e->QueryFloatAttribute("dirX", &tempParams->direction.x);
			e->QueryFloatAttribute("dirY", &tempParams->direction.y);
			e->QueryFloatAttribute("dirZ", &tempParams->direction.z);

			sceneLightManager->addSpotLight(tempParams);
		}
	}
}

/// <summary>
/// Parse the <models> node
/// </summary>
void SceneParser::parseModels(TiXmlElement* pModelRoot, std::vector<std::shared_ptr<Model>>& sceneLightingEntities)
{
	// Loop through all the elements of <models> node
	for (const TiXmlElement* e = pModelRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
	{
		// Ensure element is a <model>
		if (strcmp(e->Value(), "model") != STRCMP_SUCCESS)
			continue;

		ModelLoaderParams tempLoaderParams;

		e->QueryStringAttribute("material", &tempLoaderParams.materialID);
		e->QueryStringAttribute("mesh", &tempLoaderParams.meshID);

		e->QueryFloatAttribute("posX", &tempLoaderParams.posX);
		e->QueryFloatAttribute("posY", &tempLoaderParams.posY);
		e->QueryFloatAttribute("posZ", &tempLoaderParams.posZ);
		
		e->QueryFloatAttribute("rotX", &tempLoaderParams.rotX);
		e->QueryFloatAttribute("rotY", &tempLoaderParams.rotY);
		e->QueryFloatAttribute("rotZ", &tempLoaderParams.rotZ);

		e->QueryFloatAttribute("scaleX", &tempLoaderParams.scaleX);
		e->QueryFloatAttribute("scaleY", &tempLoaderParams.scaleY);
		e->QueryFloatAttribute("scaleZ", &tempLoaderParams.scaleZ);
		
		sceneLightingEntities.emplace_back(std::make_shared<Model>(tempLoaderParams));
	}
}

/// <summary>
/// Fill out the base light loading parameters - all lights share these common values
/// </summary>
void SceneParser::parseBaseLight(TiXmlElement* e, LightLoaderParams* pParams)
{
	e->QueryFloatAttribute("ambR", &pParams->ambient.r);
	e->QueryFloatAttribute("ambG", &pParams->ambient.g);
	e->QueryFloatAttribute("ambB", &pParams->ambient.b);

	e->QueryFloatAttribute("difR", &pParams->diffuse.r);
	e->QueryFloatAttribute("difG", &pParams->diffuse.g);
	e->QueryFloatAttribute("difB", &pParams->diffuse.b);

	e->QueryFloatAttribute("speR", &pParams->specular.r);
	e->QueryFloatAttribute("speG", &pParams->specular.g);
	e->QueryFloatAttribute("speB", &pParams->specular.b);
}
