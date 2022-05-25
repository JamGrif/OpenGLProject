#include "pch.h"
#include "Scene/SceneTextReader.h"

#include "Scene/EntityTypes/BasicEntity.h"
#include "Scene/EntityTypes/LightingEntity.h"
#include "Scene/EntityTypes/EnvironmentEntity.h"
#include "Scene/EntityTypes/TerrainEntity.h"
#include "Scene/EntityTypes/GeometryEntity.h"
#include "Scene/EntityTypes/SkyEntity.h"

#include <fstream>
#include <sstream>

#include "Scene/SceneLightManager.h"

SceneTextReader::SceneTextReader(const std::string& filename, std::vector<std::shared_ptr<BaseEntity>>& sceneNonLightingEntities, std::vector<std::shared_ptr<LightingEntity>>& sceneLightingEntities, std::shared_ptr<SceneLightManager>& sceneLightManager)
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
		else if (fullLine.at(0) == "LightingEntity") 
		{
			templateLightingEntity object;

			// Only add to list of modellighting objects to create if was read successfully from file
			if (applyToLightingEntityTemplate(object, fullLine))
				completedLightingEntityObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "BasicEntity")
		{
			templateBasicEntity object;
			
			// Only add to list of modelbasic objects to create if was read successfully from file
			if (applyToBasicEntityTemplate(object, fullLine))
				completedBasicEntityObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "TerrainEntity")
		{
			templateTerrainEntity object;
			
			// Only add to list of modelterrain objects to create if was read successfully from file
			if (applyToTerrainEntityTemplate(object, fullLine))
				completedTerrainEntityObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "EnvironmentEntity")
		{
			templateEnvironmentEntity object;
			
			// Only add to list of modelenvironment objects to create if was read successfully from file
			if (applyToEnvironmentEntityTemplate(object, fullLine))
				completedEnvironmentEntityObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "GeometryEntity")
		{
			templateGeometryEntity object;

			// Only add to list of modelgeometry objects to create if was read successfully from file
			if (applyToGeometryEntityTemplate(object, fullLine))
				completedGeometryEntityObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "SkyEntity")
		{
			templateSkyEntity object;
			
			// Only add to list of modelsky objects to create if was read successfully from file
			if (applyToSkyEntityTemplate(object, fullLine))
				completedSkyEntityObjects.emplace_back(object);
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

	for (const auto& dLightTemplate : completedDirectionalLightObjects)
	{
		sceneLightManager->addDirectionalLight(
			dLightTemplate.Ambient,
			dLightTemplate.Diffuse,
			dLightTemplate.Specular,
			dLightTemplate.Direction
		);
	}

	for (const auto& pLightTemplate : completedPointLightObjects)
	{
		sceneLightManager->addPointLight(
			pLightTemplate.Ambient,
			pLightTemplate.Diffuse,
			pLightTemplate.Specular,
			pLightTemplate.Position
		);
	}

	for (const auto& sLightTemplate : completedSpotLightObjects)
	{
		sceneLightManager->addSpotLight(
			sLightTemplate.Ambient,
			sLightTemplate.Diffuse,
			sLightTemplate.Specular,
			sLightTemplate.Position
		);
	}

	// Reserve enough space to hold all of the created entities in the vectors once (stops the vector being constantly resized as new entities are added)
	size_t newSize = completedBasicEntityObjects.size() + completedTerrainEntityObjects.size() + completedEnvironmentEntityObjects.size()
					 + completedGeometryEntityObjects.size() + completedSkyEntityObjects.size();
	sceneNonLightingEntities.reserve(newSize);

	size_t newLightingEntitySize = completedLightingEntityObjects.size();
	sceneLightingEntities.reserve(newLightingEntitySize);

	for (const auto& skyObject : completedSkyEntityObjects)
	{
		std::shared_ptr<SkyEntity> model = std::make_shared<SkyEntity>(skyObject);
		sceneNonLightingEntities.emplace_back(model);
	}

	int index = 0; // vector index - used in editor to set the entities name
	for (const auto& lightObject : completedLightingEntityObjects)
	{
		std::shared_ptr<LightingEntity> model = std::make_shared<LightingEntity>(lightObject);
		model->setEditorName("LightingEntity" + std::to_string(index));
		sceneLightingEntities.emplace_back(model);

		index++;
	}

	for (const auto& basicObject : completedBasicEntityObjects)
	{
		std::shared_ptr<BasicEntity> model = std::make_shared<BasicEntity>(basicObject);
		sceneNonLightingEntities.emplace_back(model);
	}
	
	for (const auto& terrainObject : completedTerrainEntityObjects)
	{
		std::shared_ptr<TerrainEntity> model = std::make_shared<TerrainEntity>(terrainObject);
		sceneNonLightingEntities.emplace_back(model);
	}
	
	for (const auto& environmentObject : completedEnvironmentEntityObjects)
	{
		std::shared_ptr<EnvironmentEntity> model = std::make_shared<EnvironmentEntity>(environmentObject);
		sceneNonLightingEntities.emplace_back(model);
	}
	
	for (const auto& geometryObject : completedGeometryEntityObjects)
	{
		std::shared_ptr<GeometryEntity> model = std::make_shared<GeometryEntity>(geometryObject);
		sceneNonLightingEntities.emplace_back(model);
	}

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
		PRINT_WARN("SCENETEXTREADER-> Failed to add a point light from textfile (line is incorrect length)");
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add point light from textfile ({0})", e.what());
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add a directional light from textfile (line is incorrect length)");
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add directional light from textfile ({0})", e.what());
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add a spot light from textfile (line is incorrect length)");
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add spot light from textfile ({0})", e.what());
		return false;
	}

	return true;
}

void SceneTextReader::applyToBaseEntity(templateBaseEntity& o, const std::vector<std::string>& fullLine)
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

	o.position.x = std::stof(fullLine.at(e_PosX));
	o.position.y = std::stof(fullLine.at(e_PosY));
	o.position.z = std::stof(fullLine.at(e_PosZ));

	o.rotation.x = std::stof(fullLine.at(e_RotX));
	o.rotation.y = std::stof(fullLine.at(e_RotY));
	o.rotation.z = std::stof(fullLine.at(e_RotZ));

	o.scale.x = std::stof(fullLine.at(e_ScaleX));
	o.scale.y = std::stof(fullLine.at(e_ScaleY));
	o.scale.z = std::stof(fullLine.at(e_ScaleZ));
}


bool SceneTextReader::applyToLightingEntityTemplate(templateLightingEntity& o, const std::vector<std::string>& fullLine)
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

		e_END_OF_MODELLIGHTING_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELLIGHTING_ENUM)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelLighting from textfile (line is incorrect length)");
		return false;
	}

	try
	{
		applyToBaseEntity(o, fullLine);

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
	}
	catch (const std::exception& e)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelLighting from textfile ({0})", e.what());
		return false;
	}

	return true;
}

bool SceneTextReader::applyToBasicEntityTemplate(templateBasicEntity& o, const std::vector<std::string>& fullLine)
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelBasic from textfile (line is incorrect length)");
		return false;
	}

	try
	{
		applyToBaseEntity(o, fullLine);

		o.mesh = "res/meshes/" + fullLine.at(e_mesh) + ".obj";

		o.lightToCopy = std::stoi(fullLine.at(e_lightToCopy));
	}
	catch (const std::exception& e)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelBasic from textfile ({0})", e.what());
		return false;
	}

	return true;
}

bool SceneTextReader::applyToTerrainEntityTemplate(templateTerrainEntity& o, const std::vector<std::string>& fullLine)
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelTerrain from textfile (line is incorrect length)");
		return false;
	}

	try
	{
		applyToBaseEntity(o, fullLine);

		o.elevation = std::stof(fullLine.at(e_Elevation));

		o.terrainTextureMap = "res/textures/" + fullLine.at(e_TerrainTextureMap) + ".png";
		o.terrainHeightMap = "res/textures/" + fullLine.at(e_TerrainHeightMap) + ".png";
	}
	catch (const std::exception& e)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelTerrain from textfile ({0})", e.what());
		return false;
	}

	return true;
}

bool SceneTextReader::applyToEnvironmentEntityTemplate(templateEnvironmentEntity& o, const std::vector<std::string>& fullLine)
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
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelEnvironment from textfile (line is incorrect length)");
		return false;
	}

	try
	{
		applyToBaseEntity(o, fullLine);

		o.mesh = "res/meshes/" + fullLine.at(e_mesh) + ".obj";

		o.reflection = std::stof(fullLine.at(e_reflection));

		o.refraction = std::stof(fullLine.at(e_refraction));
	}
	catch (const std::exception& e)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelEnvironment from textfile ({0})", e.what());
		return false;
	}

	return true;
}

bool SceneTextReader::applyToGeometryEntityTemplate(templateGeometryEntity& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_END_OF_MODEGEOMETRY_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODEGEOMETRY_ENUM)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelGeometry from textfile (line is incorrect length)");
		return false;
	}

	try
	{
		applyToBaseEntity(o, fullLine);

		o.mesh = "res/meshes/" + fullLine.at(e_mesh) + ".obj";
	}
	catch (const std::exception& e)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelGeometry from textfile ({0})", e.what());
		return false;
	}

	return true;
}

bool SceneTextReader::applyToSkyEntityTemplate(templateSkyEntity& o, const std::vector<std::string>& fullLine)
{
	enum objectInfo
	{
		e_skyboxTexture = 10,

		e_END_OF_MODELSKY_ENUM
	};

	// Ensure text line is correct size
	if (fullLine.size() != e_END_OF_MODELSKY_ENUM)
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelSky from textfile (line is incorrect length)");
		return false;
	}

	try
	{
		applyToBaseEntity(o, fullLine);

		o.skyboxTexture = fullLine.at(e_skyboxTexture);
	}
	catch (const std::exception& e )
	{
		PRINT_WARN("SCENETEXTREADER-> Failed to add modelSky from textfile ({0})", e.what());
		return false;
	}

	return true;
}
