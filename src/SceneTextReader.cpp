#include "pch.h"

#include "SceneTextReader.h"

#include "models/ModelBasic.h"
#include "models/ModelLighting.h"
#include "models/ModelEnvironment.h"
#include "models/ModelSprite.h"
#include "models/ModelTerrain.h"
#include "models/ModelGeometry.h"
#include "models/ModelSky.h"

#include <fstream>
#include <sstream>

#include "LightManager.h"

void SceneTextReader::createLights(std::shared_ptr<LightManager> sceneLightManager, std::vector<templatePointLight> completedPointLightObjects, std::vector<templateDirectionalLight> completedDirectionalLightObjects, std::vector<templateSpotLight> completedSpotLightObjects)
{
	for (const auto& o : completedDirectionalLightObjects)
	{
		sceneLightManager->addDirectionalLight(
			o.Ambient,
			o.Diffuse,
			o.Specular,
			o.Direction
		);
	}
	
	for (const auto& o : completedPointLightObjects)
	{
		sceneLightManager->addPointLight(
			o.Ambient,
			o.Diffuse,
			o.Specular,
			o.Position
		);
	}
	
	for (const auto& o : completedSpotLightObjects)
	{
		sceneLightManager->addSpotLight(
			o.Ambient,
			o.Diffuse,
			o.Specular,
			o.Position
		);
	}

}


SceneTextReader::SceneTextReader(const std::string& filename, std::vector<std::shared_ptr<Model>>& sceneMeshes, std::shared_ptr<LightManager>& sceneLightManager)
	:m_filename(filename), m_status(false)
{
	std::ifstream fileStream(m_filename, std::ios::in);

	if (!fileStream) // Check if file opened correctly
	{
		std::cout << "Scene textfile not found - " << filename << std::endl;
		m_status = false;
		return;
	}

	/*
		1. Read the text file, determining all the objectTypes present in the scene and fill out temp objects
		   to be used in there construction in the next part
	*/

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

		if (fullLine.at(0) == "directionalLight") // .at(0) is the first word in the row - ObjectType
		{
			templateDirectionalLight object;

			// Only add to list of directional lights to create if was read successfully from file
			if (applyToDirectionalLight(object, fullLine))
				completedDirectionalLightObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "pointLight")
		{
			templatePointLight object;

			// Only add to list of point lights to create if was read successfully from file
			if (applyToPointLight(object, fullLine))
				completedPointLightObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "spotLight")
		{
			templateSpotLight object;

			// Only add to list of spot lights to create if was read successfully from file
			if (applyToSpotLight(object, fullLine))
				completedSpotLightObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelLighting") 
		{
			templateModelLighting object;

			// Only add to list of modellighting objects to create if was read successfully from file
			if (applyToModelLightingTemplate(object, fullLine))
				completedModelLightObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelBasic")
		{
			templateModelBasic object;

			// Only add to list of modelbasic objects to create if was read successfully from file
			if (applyToModelBasicTemplate(object, fullLine))
				completedModelBasicObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelTerrain")
		{
			templateModelTerrain object;

			// Only add to list of modelterrain objects to create if was read successfully from file
			if (applyToModelTerrainTemplate(object, fullLine))
				completedModelTerrainObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelSprite")
		{
			templateModelSprite object;

			// Only add to list of modelsprite objects to create if was read successfully from file
			if (applyToModelSpriteTemplate(object, fullLine))
				completedModelSpriteObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelEnvironment")
		{
			templateModelEnvironment object;

			// Only add to list of modelenvironment objects to create if was read successfully from file
			if (applyToModelEnvironmentTemplate(object, fullLine))
				completedModelEnvironmentObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelGeometry")
		{
			templateModelGeometry object;

			// Only add to list of modelgeometry objects to create if was read successfully from file
			if (applyToModelGeometryTemplate(object, fullLine))
				completedModelGeometryObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelSky")
		{
			templateModelSky object;

			// Only add to list of modelsky objects to create if was read successfully from file
			if (applyToModelSkyTemplate(object, fullLine))
				completedModelSkyObjects.emplace_back(object);
		}
		else
		{
			std::cout << "Could not determine objectType - " << fullLine.at(0) << " - FAILURE" << std::endl;
		}

	}// end of scene txt file read

	fileStream.close();

	

	/*
		2. Use the temp objects to create and fill out the attributes of all the required objects in the scene
		   and add it to the scene vector
	*/

	//std::thread worker(&SceneTextReader::createLights, this, sceneLightManager, completedPointLightObjects, completedDirectionalLightObjects, completedSpotLightObjects);

	for (const auto& o : completedDirectionalLightObjects)
	{
		sceneLightManager->addDirectionalLight(
			o.Ambient,
			o.Diffuse,
			o.Specular,
			o.Direction
		);
	}

	for (const auto& o : completedPointLightObjects)
	{
		sceneLightManager->addPointLight(
			o.Ambient,
			o.Diffuse,
			o.Specular,
			o.Position
		);
	}

	for (const auto& o : completedSpotLightObjects)
	{
		sceneLightManager->addSpotLight(
			o.Ambient,
			o.Diffuse,
			o.Specular,
			o.Position
		);
	}


	for (const auto& o : completedModelSkyObjects)
	{
		std::shared_ptr<ModelSky> model = std::make_shared<ModelSky>();

		model->SetXPos(o.PosX);
		model->SetYPos(o.PosY);
		model->SetZPos(o.PosZ);

		model->SetXRot(o.RotX);
		model->SetYRot(o.RotY);
		model->SetZRot(o.RotZ);

		model->SetXScale(o.ScaleX);
		model->SetYScale(o.ScaleY);
		model->SetZScale(o.ScaleZ);

		model->setSkyboxTexture(o.skyboxTexture);

		sceneMeshes.emplace_back(model);
	}

	

	for (const auto& o : completedModelLightObjects)
	{
		std::shared_ptr<ModelLighting> model = std::make_shared<ModelLighting>();
	
		model->SetXPos(o.PosX);
		model->SetYPos(o.PosY);
		model->SetZPos(o.PosZ);
	
		model->SetXRot(o.RotX);
		model->SetYRot(o.RotY);
		model->SetZRot(o.RotZ);
	
		model->SetXScale(o.ScaleX);
		model->SetYScale(o.ScaleY);
		model->SetZScale(o.ScaleZ);
	
		model->setMesh(o.mesh);
	
		if (o.diffuseMap != "null")
		{
			model->setDiffuseTexture(o.diffuseMap);
		}
		else
		{
			model->setDiffuseTexture("res/textures/blank.png"); // Models have to have a diffuse map
		}
	
		if (o.specularMap != "null")
		{
			model->setSpecularTexture(o.specularMap, 48.0f);
		}
		else
		{
			model->setSpecularTexture("res/textures/blank.png", 48.0f); // Models have to have a specular map
		}
	
		if (o.normalMap != "null")
		{
			model->setNormalTexture(o.normalMap, o.normalMapNormalize);
		}
	
		if (o.heightMap != "null")
		{
			model->setHeightTexture(o.heightMap, o.heightMapHeight);
		}
	
		if (o.emissionMap != "null")
		{
			model->setEmissionTexture(o.emissionMap);
		}

		model->affectedByGravity(o.gravity);
	
		sceneMeshes.emplace_back(model);
	}

	for (const auto& o : completedModelBasicObjects)
	{
		std::shared_ptr<ModelBasic> model = std::make_shared<ModelBasic>();

		model->SetXPos(o.PosX);
		model->SetYPos(o.PosY);
		model->SetZPos(o.PosZ);

		model->SetXRot(o.RotX);
		model->SetYRot(o.RotY);
		model->SetZRot(o.RotZ);

		model->SetXScale(o.ScaleX);
		model->SetYScale(o.ScaleY);
		model->SetZScale(o.ScaleZ);

		model->setMesh(o.mesh);

		model->copyPointLight(o.lightToCopy);

		sceneMeshes.emplace_back(model);
	}

	
	for (const auto& o : completedModelTerrainObjects)
	{
		std::shared_ptr<ModelTerrain> model = std::make_shared<ModelTerrain>();

		model->SetXPos(o.PosX);
		model->SetYPos(o.PosY);
		model->SetZPos(o.PosZ);

		model->SetXRot(o.RotX);
		model->SetYRot(o.RotY);
		model->SetZRot(o.RotZ);

		model->SetXScale(o.ScaleX);
		model->SetYScale(o.ScaleY);
		model->SetZScale(o.ScaleZ);

		model->setElevation(o.elevation);

		model->setTerrainTexture(o.terrainTextureMap);
		model->setTerrainHeightTexture(o.terrainHeightMap);

		sceneMeshes.emplace_back(model);
	}

	for (const auto& o : completedModelSpriteObjects)
	{
		std::shared_ptr<ModelSprite> model = std::make_shared<ModelSprite>();

		model->SetXPos(o.PosX);
		model->SetYPos(o.PosY);
		model->SetZPos(o.PosZ);

		model->SetXRot(o.RotX);
		model->SetYRot(o.RotY);
		model->SetZRot(o.RotZ);

		model->SetXScale(o.ScaleX);
		model->SetYScale(o.ScaleY);
		model->SetZScale(o.ScaleZ);

		model->setSprite(o.sprite);

		sceneMeshes.emplace_back(model);
	}

	for (const auto& o : completedModelEnvironmentObjects)
	{
		std::shared_ptr<ModelEnvironment> model = std::make_shared<ModelEnvironment>();

		model->SetXPos(o.PosX);
		model->SetYPos(o.PosY);
		model->SetZPos(o.PosZ);

		model->SetXRot(o.RotX);
		model->SetYRot(o.RotY);
		model->SetZRot(o.RotZ);

		model->SetXScale(o.ScaleX);
		model->SetYScale(o.ScaleY);
		model->SetZScale(o.ScaleZ);

		model->setMesh(o.mesh);

		model->toggleReflection(o.reflection);
		model->toggleRefraction(o.refraction);

		sceneMeshes.emplace_back(model);
	}

	for (const auto& o : completedModelGeometryObjects)
	{
		std::shared_ptr<ModelGeometry> model = std::make_shared<ModelGeometry>();

		model->SetXPos(o.PosX);
		model->SetYPos(o.PosY);
		model->SetZPos(o.PosZ);

		model->SetXRot(o.RotX);
		model->SetYRot(o.RotY);
		model->SetZRot(o.RotZ);

		model->SetXScale(o.ScaleX);
		model->SetYScale(o.ScaleY);
		model->SetZScale(o.ScaleZ);

		model->setMesh(o.mesh);

		sceneMeshes.emplace_back(model);
	}

	//worker.join();

	m_status = true;
}

SceneTextReader::~SceneTextReader()
{
}

bool SceneTextReader::getStatus()
{
	return m_status;
}

void SceneTextReader::applyToLight(templateLight& l, const std::vector<std::string>& fullLine)
{
	enum objectInfo
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

	l.modelType = fullLine.at(e_objectType);

	l.Ambient.r = std::stof(fullLine.at(e_AmbR));
	l.Ambient.g = std::stof(fullLine.at(e_AmbG));
	l.Ambient.b = std::stof(fullLine.at(e_ambB));

	l.Diffuse.r = std::stof(fullLine.at(e_DiffR));
	l.Diffuse.g = std::stof(fullLine.at(e_DiffG));
	l.Diffuse.b = std::stof(fullLine.at(e_DiffB));

	l.Specular.r = std::stof(fullLine.at(e_SpecR));
	l.Specular.g = std::stof(fullLine.at(e_SpecG));
	l.Specular.b = std::stof(fullLine.at(e_specB));
}


bool SceneTextReader::applyToPointLight(templatePointLight& l, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_PositionX = 10,
		e_PositionY = 11,
		e_PositionZ = 12,

		e_END_OF_POINT_LIGHT_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_POINT_LIGHT_ENUM)
	{
		std::cout << "SCENE->failed to add point light from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToLight(l, fullLine);

		l.Position.x = std::stof(fullLine.at(e_PositionX));
		l.Position.y = std::stof(fullLine.at(e_PositionY));
		l.Position.z = std::stof(fullLine.at(e_PositionZ));
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add point light from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}


bool SceneTextReader::applyToDirectionalLight(templateDirectionalLight& l, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_DirectionX = 10,
		e_DirectionY = 11,
		e_DirectionZ = 12,

		e_END_OF_DIRECTIONAL_LIGHT_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_DIRECTIONAL_LIGHT_ENUM)
	{
		std::cout << "SCENE->failed to add directional light from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToLight(l, fullLine);

		l.Direction.x = std::stof(fullLine.at(e_DirectionX));
		l.Direction.y = std::stof(fullLine.at(e_DirectionY));
		l.Direction.z = std::stof(fullLine.at(e_DirectionZ));
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add directional light from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}


bool SceneTextReader::applyToSpotLight(templateSpotLight& l, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_PositionX = 10,
		e_PositionY = 11,
		e_PositionZ = 12,

		e_END_OF_SPOT_LIGHT_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_SPOT_LIGHT_ENUM)
	{
		std::cout << "SCENE->failed to add spot light from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToLight(l, fullLine);

		l.Position.x = std::stof(fullLine.at(e_PositionX));
		l.Position.y = std::stof(fullLine.at(e_PositionY));
		l.Position.z = std::stof(fullLine.at(e_PositionZ));
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add spot light from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}

void SceneTextReader::applyToModel(templateModel& o, const std::vector<std::string>& fullLine)
{
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
	};

	o.objectType = fullLine.at(e_objectType);

	o.PosX = std::stof(fullLine.at(e_PosX));
	o.PosY = std::stof(fullLine.at(e_PosY));
	o.PosZ = std::stof(fullLine.at(e_PosZ));

	o.RotX = std::stof(fullLine.at(e_RotX));
	o.RotY = std::stof(fullLine.at(e_RotY));
	o.RotZ = std::stof(fullLine.at(e_RotZ));

	o.ScaleX = std::stof(fullLine.at(e_ScaleX));
	o.ScaleY = std::stof(fullLine.at(e_ScaleY));
	o.ScaleZ = std::stof(fullLine.at(e_ScaleZ));
}


bool SceneTextReader::applyToModelLightingTemplate(templateModelLighting& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_mesh = 10,
		e_diffuseMap = 11,
		e_specularMap = 12,
		e_normalMap = 13,
		e_normalMapNormalize = 14,
		e_heightMap = 15,
		e_heightMapHeight = 16,
		e_emissionMap = 17,
		e_gravity = 18,

		e_END_OF_MODELLIGHTING_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELLIGHTING_ENUM)
	{
		std::cout << "SCENE->failed to add modelLighting from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToModel(o, fullLine);

		o.mesh = "res/meshes/" + fullLine.at(e_mesh) + ".obj";

		if (fullLine.at(e_diffuseMap) == "null")
		{
			o.diffuseMap = "null"; // Object will use no diffuse map
		}
		else
		{
			o.diffuseMap = "res/textures/" + fullLine.at(e_diffuseMap) + ".png";
		}

		if (fullLine.at(e_specularMap) == "null")
		{
			o.specularMap = "null"; // Object will use no specular map
		}
		else
		{
			o.specularMap = "res/textures/" + fullLine.at(e_specularMap) + ".png";
		}

		if (fullLine.at(e_normalMap) == "null")
		{
			o.normalMap = "null"; // Object will use no normal map
		}
		else
		{
			o.normalMap = "res/textures/" + fullLine.at(e_normalMap) + ".png";
		}

		o.normalMapNormalize = std::stof(fullLine.at(e_normalMapNormalize));

		if (fullLine.at(e_heightMap) == "null")
		{
			o.heightMap = "null"; // Object will use no height map
		}
		else
		{
			o.heightMap = "res/textures/" + fullLine.at(e_heightMap) + ".png";
		}

		o.heightMapHeight = std::stof(fullLine.at(e_heightMapHeight));

		if (fullLine.at(e_emissionMap) == "null")
		{
			o.emissionMap = "null"; // Object will use no emission map
		}
		else
		{
			o.emissionMap = "res/textures/" + fullLine.at(e_emissionMap) + ".png";
		}

		o.gravity = std::stof(fullLine.at(e_gravity));
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add modelLighting from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}

bool SceneTextReader::applyToModelBasicTemplate(templateModelBasic& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_lightToCopy = 11,

		e_END_OF_MODELBASIC_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELBASIC_ENUM)
	{
		std::cout << "SCENE->failed to add modelBasic from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToModel(o, fullLine);

		o.mesh = "res/meshes/" + fullLine.at(e_mesh) + ".obj";

		o.lightToCopy = std::stoi(fullLine.at(e_lightToCopy));
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add modelBasic from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}




	return true;
}


bool SceneTextReader::applyToModelTerrainTemplate(templateModelTerrain& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_Elevation = 10,

		e_TerrainTextureMap = 11,
		e_TerrainHeightMap = 12,

		e_END_OF_MODELTERRAIN_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELTERRAIN_ENUM)
	{
		std::cout << "SCENE->failed to add modelTerrain from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToModel(o, fullLine);

		o.elevation = std::stof(fullLine.at(e_Elevation));

		o.terrainTextureMap = "res/textures/" + fullLine.at(e_TerrainTextureMap) + ".png";
		o.terrainHeightMap = "res/textures/" + fullLine.at(e_TerrainHeightMap) + ".png";
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add modelTerrain from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}

bool SceneTextReader::applyToModelSpriteTemplate(templateModelSprite& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_Sprite = 10,

		e_END_OF_MODELSPRITE_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELSPRITE_ENUM)
	{
		std::cout << "SCENE->failed to add modelSprite from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToModel(o, fullLine);

		o.sprite = "res/textures/" + fullLine.at(e_Sprite) + ".png";
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add modelSprite from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}

bool SceneTextReader::applyToModelEnvironmentTemplate(templateModelEnvironment& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_reflection = 11,

		e_refraction = 12,

		e_END_OF_MODELENVIRONMENT_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELENVIRONMENT_ENUM)
	{
		std::cout << "SCENE->failed to add modelEnvironment from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToModel(o, fullLine);

		o.mesh = "res/meshes/" + fullLine.at(e_mesh) + ".obj";

		o.reflection = std::stof(fullLine.at(e_reflection));

		o.refraction = std::stof(fullLine.at(e_refraction));
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add modelEnvironment from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}

bool SceneTextReader::applyToModelGeometryTemplate(templateModelGeometry& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_END_OF_MODEGEOMETRY_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODEGEOMETRY_ENUM)
	{
		std::cout << "SCENE->failed to add modelGeometry from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToModel(o, fullLine);

		o.mesh = "res/meshes/" + fullLine.at(e_mesh) + ".obj";
	}
	catch (const std::exception& e)
	{
		std::cout << "SCENE->failed to add modelGeometry from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}

bool SceneTextReader::applyToModelSkyTemplate(templateModelSky& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_skyboxTexture = 10,

		e_END_OF_MODELSKY_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELSKY_ENUM)
	{
		std::cout << "SCENE->failed to add modelSky from textfile (line is incorrect length) - FAILURE" << std::endl;
		return false;
	}

	try
	{
		applyToModel(o, fullLine);

		o.skyboxTexture = fullLine.at(e_skyboxTexture);
	}
	catch (const std::exception& e )
	{
		std::cout << "SCENE->failed to add modelSky from textfile (" << e.what() << ") - FAILURE" << std::endl;
		return false;
	}

	return true;
}
