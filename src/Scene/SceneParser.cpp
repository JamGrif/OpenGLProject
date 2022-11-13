#include "pch.h"
#include "Scene/SceneParser.h"

#include "Rendering/Model.h"
#include "Scene/SceneSky.h"

#include <fstream>
#include <sstream>

#include "Scene/SceneLightManager.h"

#include "Rendering/MaterialManager.h"
#include "Rendering/TextureManager.h"


SceneParser::SceneParser(const std::string& filename, std::vector<std::shared_ptr<Model>>& sceneLightingEntities, std::shared_ptr<SceneLightManager>& sceneLightManager, SceneSky** sceneSky)
	:m_status(false)
{
	std::ifstream fileStream(filename, std::ios::in);

	if (!fileStream) // Check if file opened correctly
	{
		PRINT_WARN("SCENETEXTREADER-> textfile not found - {0}", filename);
		m_status = false;
		return;
	}

	/*
		1. Read the text file, determining all the objectTypes present in the scene and fill out temp objects
		   to be used in there construction in the next part
	*/

	int number = 0; // vector index - used in editor to set the entities name
	while (!fileStream.eof())
	{
		std::string textFileLine = "";
		std::getline(fileStream, textFileLine);

		// Check if line is empty or a comment
		if (textFileLine == "" || textFileLine.at(0) == '#')
		{
			// Skip line
			continue;
		}

		std::string buf;
		std::stringstream ss(textFileLine);
		std::vector<std::string> fullLine; // Full line, with each word as separate strings

		// Continuously read the textfile line, putting each word individually into the vector
		while (ss >> buf)
		{
			fullLine.push_back(buf);
		}

		/*
			Determine what objectType the textfile line is and create and fill out a temp template type for that objectType
			which will be used to actually create the objects later on
		*/

		// .at(0) is the first word in the row
		
		if (fullLine.at(0) == "material")
		{

			MaterialLoaderParams tempLoaderParams;
			tempLoaderParams.materialID = fullLine.at(1);

			tempLoaderParams.diffuseMapID = fullLine.at(2);
			tempLoaderParams.diffuseMapPath = "res/textures/" + fullLine.at(2) + ".png";

			tempLoaderParams.specularMapID = fullLine.at(3);
			tempLoaderParams.specularMapPath = "res/textures/" + fullLine.at(3) + ".png";

			tempLoaderParams.normalMapID = fullLine.at(4);
			tempLoaderParams.normalMapPath = "res/textures/" + fullLine.at(4) + ".png";

			tempLoaderParams.normalMapNormalize = std::stof(fullLine.at(5));

			tempLoaderParams.heightMapID = fullLine.at(6);
			tempLoaderParams.heightMapPath = "res/textures/" + fullLine.at(6) + ".png";

			tempLoaderParams.heightMapHeight = std::stof(fullLine.at(7));

			tempLoaderParams.emissionMapID = fullLine.at(8);
			tempLoaderParams.emissionMapPath = "res/textures/" + fullLine.at(8) + ".png";

			TheMaterialManager::Instance()->createMaterial(fullLine.at(1), tempLoaderParams);

		}
		else if (fullLine.at(0) == "SceneSky")
		{
			*sceneSky = new SceneSky();

			(*sceneSky)->setCubemapID(fullLine.at(1));
			TheTextureManager::Instance()->createCubemap(fullLine.at(1), fullLine.at(1));
		}
		else if (fullLine.at(0) == "directionalLight") 
		{
			enum objectInfo1
			{
				e_objectType = 0,

				e_AmbR = 1,
				e_AmbG = 2,
				e_ambB = 3,

				e_DiffR = 4,
				e_DiffG = 5,
				e_DiffB = 6,

				e_SpecR = 7,
				e_SpecG = 8,
				e_specB = 9
			};
			enum objectInfo2
			{
				e_DirectionX = 10,
				e_DirectionY = 11,
				e_DirectionZ = 12,

				e_END_OF_DIRECTIONAL_LIGHT_ENUM
			};

			DirectionalLoaderParams* tempParams = new DirectionalLoaderParams;

			tempParams->ambient.r = std::stof(fullLine.at(e_AmbR));
			tempParams->ambient.g = std::stof(fullLine.at(e_AmbG));
			tempParams->ambient.b = std::stof(fullLine.at(e_ambB));

			tempParams->diffuse.r = std::stof(fullLine.at(e_DiffR));
			tempParams->diffuse.g = std::stof(fullLine.at(e_DiffG));
			tempParams->diffuse.b = std::stof(fullLine.at(e_DiffB));

			tempParams->specular.r = std::stof(fullLine.at(e_SpecR));
			tempParams->specular.g = std::stof(fullLine.at(e_SpecG));
			tempParams->specular.b = std::stof(fullLine.at(e_specB));

			tempParams->direction.x = std::stof(fullLine.at(e_DirectionX));
			tempParams->direction.y = std::stof(fullLine.at(e_DirectionY));
			tempParams->direction.z = std::stof(fullLine.at(e_DirectionZ));


			sceneLightManager->addDirectionalLight(tempParams);
		}
		else if (fullLine.at(0) == "pointLight")
		{
			enum objectInfo1
			{
				e_objectType = 0,

				e_AmbR = 1,
				e_AmbG = 2,
				e_ambB = 3,

				e_DiffR = 4,
				e_DiffG = 5,
				e_DiffB = 6,

				e_SpecR = 7,
				e_SpecG = 8,
				e_specB = 9
			};
			enum objectInfo2
			{
				e_PositionX = 10,
				e_PositionY = 11,
				e_PositionZ = 12,

				e_END_OF_POINT_LIGHT_ENUM
			};

			PointLoaderParams* tempParams = new PointLoaderParams;

			tempParams->ambient.r = std::stof(fullLine.at(e_AmbR));
			tempParams->ambient.g = std::stof(fullLine.at(e_AmbG));
			tempParams->ambient.b = std::stof(fullLine.at(e_ambB));

			tempParams->diffuse.r = std::stof(fullLine.at(e_DiffR));
			tempParams->diffuse.g = std::stof(fullLine.at(e_DiffG));
			tempParams->diffuse.b = std::stof(fullLine.at(e_DiffB));

			tempParams->specular.r = std::stof(fullLine.at(e_SpecR));
			tempParams->specular.g = std::stof(fullLine.at(e_SpecG));
			tempParams->specular.b = std::stof(fullLine.at(e_specB));

			tempParams->position.x = std::stof(fullLine.at(e_PositionX));
			tempParams->position.y = std::stof(fullLine.at(e_PositionY));
			tempParams->position.z = std::stof(fullLine.at(e_PositionZ));

			sceneLightManager->addPointLight(tempParams);
		}
		else if (fullLine.at(0) == "spotLight")
		{

			enum objectInfo1
			{
				e_objectType = 0,

				e_AmbR = 1,
				e_AmbG = 2,
				e_ambB = 3,

				e_DiffR = 4,
				e_DiffG = 5,
				e_DiffB = 6,

				e_SpecR = 7,
				e_SpecG = 8,
				e_specB = 9
			};
			enum objectInfo2
			{
				e_PositionX = 10,
				e_PositionY = 11,
				e_PositionZ = 12,

				e_END_OF_SPOT_LIGHT_ENUM
			};

			SpotLoaderParams* tempParams = new SpotLoaderParams;

			tempParams->ambient.r = std::stof(fullLine.at(e_AmbR));
			tempParams->ambient.g = std::stof(fullLine.at(e_AmbG));
			tempParams->ambient.b = std::stof(fullLine.at(e_ambB));

			tempParams->diffuse.r = std::stof(fullLine.at(e_DiffR));
			tempParams->diffuse.g = std::stof(fullLine.at(e_DiffG));
			tempParams->diffuse.b = std::stof(fullLine.at(e_DiffB));

			tempParams->specular.r = std::stof(fullLine.at(e_SpecR));
			tempParams->specular.g = std::stof(fullLine.at(e_SpecG));
			tempParams->specular.b = std::stof(fullLine.at(e_specB));

			tempParams->position.x = std::stof(fullLine.at(e_PositionX));
			tempParams->position.y = std::stof(fullLine.at(e_PositionY));
			tempParams->position.z = std::stof(fullLine.at(e_PositionZ));

			sceneLightManager->addSpotLight(tempParams);


		}
		else if (fullLine.at(0) == "LightingEntity") 
		{
			//templateLightingEntity object;

			// Only add to list of modellighting objects to create if was read successfully from file
			enum objectInfo
			{
				e_objectType = 0,

				e_PosX = 1,
				e_PosY = 2,
				e_PosZ = 3,
				e_RotX = 4,
				e_RotY = 5,
				e_RotZ = 6,
				e_ScaleX = 7,
				e_ScaleY = 8,
				e_ScaleZ = 9,

				e_MaterialID = 10,
			};


			ModelLoaderParams tempLoaderParams;
			tempLoaderParams.objectType = fullLine.at(e_objectType);

			tempLoaderParams.position.x = std::stof(fullLine.at(e_PosX));
			tempLoaderParams.position.y = std::stof(fullLine.at(e_PosY));
			tempLoaderParams.position.z = std::stof(fullLine.at(e_PosZ));

			tempLoaderParams.rotation.x = std::stof(fullLine.at(e_RotX));
			tempLoaderParams.rotation.y = std::stof(fullLine.at(e_RotY));
			tempLoaderParams.rotation.z = std::stof(fullLine.at(e_RotZ));

			tempLoaderParams.scale.x = std::stof(fullLine.at(e_ScaleX));
			tempLoaderParams.scale.y = std::stof(fullLine.at(e_ScaleY));
			tempLoaderParams.scale.z = std::stof(fullLine.at(e_ScaleZ));

			tempLoaderParams.materialID = fullLine.at(e_MaterialID);

			tempLoaderParams.meshID = fullLine.at(11);
			tempLoaderParams.meshFilepath = "res/meshes/" + fullLine.at(11) + ".obj";


			std::shared_ptr<Model> model = std::make_shared<Model>(tempLoaderParams);

			
			model->setEditorName("Model" + std::to_string(number++));
			sceneLightingEntities.emplace_back(model);
		}
		else
		{
			PRINT_WARN("SCENETEXTREADER-> Count not determine objectType - {0}", fullLine.at(0));
		}

	}// end of scene txt file read

	fileStream.close();

	/*
		2. Use the temp objects to create and fill out the attributes of all the required objects in the scene
		   and add it to the scene vector
	*/

	

	m_status = true;
}

SceneParser::~SceneParser()
{
}

