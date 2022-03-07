#include "SceneTextReader.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "LightManager.h"

SceneTextReader::SceneTextReader(const std::string& filename, std::vector<Model*>& sceneMeshes, LightManager* sceneLightManager)
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

		if (fullLine.at(0) == "directionalLight")
		{
			templateDirectionalLight object;
			applyToDirectionalLight(object, fullLine);
			completedDirectionalLightObjects.push_back(object);
		}
		else if (fullLine.at(0) == "pointLight")
		{
			templatePointLight object;
			applyToPointLight(object, fullLine);
			completedPointLightObjects.push_back(object);
		}
		else if (fullLine.at(0) == "spotLight")
		{
			templateSpotLight object;
			applyToSpotLight(object, fullLine);
			completedSpotLightObjects.push_back(object);
		}
		else if (fullLine.at(0) == "modelLighting") // .at(0) is the first word in the row - ObjectType
		{
			templateModelLighting object;
			applyToModelLightingTemplate(object, fullLine);
			completedModelLightObjects.emplace_back(object);
		}
		else if (fullLine.at(0) == "modelBasic")
		{
			templateModelBasic object;
			applyToModelBasicTemplate(object, fullLine);
			completedModelBasicObjects.push_back(object);
		}
		else if (fullLine.at(0) == "modelTerrain")
		{
			templateModelTerrain object;
			applyToModelTerrainTemplate(object, fullLine);
			completedModelTerrainObjects.push_back(object);
		}
		else if (fullLine.at(0) == "modelSprite")
		{
			templateModelSprite object;
			applyToModelSpriteTemplate(object, fullLine);
			completedModelSpriteObjects.push_back(object);
		}
		else if (fullLine.at(0) == "modelEnvironment")
		{
			templateModelEnvironment object;
			applyToModelEnvironmentTemplate(object, fullLine);
			completedModelEnvironmentObjects.push_back(object);
		}
		else if (fullLine.at(0) == "modelGeometry")
		{
			templateModelGeometry object;
			applyToModelGeometryTemplate(object, fullLine);
			completedModelGeometryObjects.push_back(object);
		}
		else if (fullLine.at(0) == "modelSky")
		{
			templateModelSky object;
			applyToModelSkyTemplate(object, fullLine);
			completedModelSkyObjects.push_back(object);
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
		ModelSky* model = new ModelSky();

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

		sceneMeshes.push_back(model);
	}

	for (const auto& o : completedModelLightObjects)
	{
		ModelLighting* model = new ModelLighting();

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
			model->setSpecularTexture(o.specularMap);
		}
		else
		{
			model->setSpecularTexture("res/textures/blank.png"); // Models have to have a specular map
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

		sceneMeshes.push_back(model);
	}

	for (const auto& o : completedModelBasicObjects)
	{
		ModelBasic* model = new ModelBasic();

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

		sceneMeshes.push_back(model);
	}

	for (const auto& o : completedModelTerrainObjects)
	{
		ModelTerrain* model = new ModelTerrain();

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

		sceneMeshes.push_back(model);
	}

	for (const auto& o : completedModelSpriteObjects)
	{
		ModelSprite* model = new ModelSprite();

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
		sceneMeshes.push_back(model);
	}

	for (const auto& o : completedModelEnvironmentObjects)
	{
		ModelEnvironment* model = new ModelEnvironment();

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

		sceneMeshes.push_back(model);
	}

	for (const auto& o : completedModelGeometryObjects)
	{
		ModelGeometry* model = new ModelGeometry();

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

		sceneMeshes.push_back(model);
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

void SceneTextReader::applyToLight(templateLight& l, const std::vector<std::string>& vector)
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

	l.modelType = vector.at(e_objectType);

	l.Ambient.r = std::stof(vector.at(e_AmbR));
	l.Ambient.g = std::stof(vector.at(e_AmbG));
	l.Ambient.b = std::stof(vector.at(e_ambB));

	l.Diffuse.r = std::stof(vector.at(e_DiffR));
	l.Diffuse.g = std::stof(vector.at(e_DiffG));
	l.Diffuse.b = std::stof(vector.at(e_DiffB));

	l.Specular.r = std::stof(vector.at(e_SpecR));
	l.Specular.g = std::stof(vector.at(e_SpecG));
	l.Specular.b = std::stof(vector.at(e_specB));
}


void SceneTextReader::applyToPointLight(templatePointLight& l, const std::vector<std::string>& vector)
{
	applyToLight(l, vector);

	enum objectInfo
	{
		e_PositionX = 10,
		e_PositionY = 11,
		e_PositionZ = 12
	};

	l.Position.x = std::stof(vector.at(e_PositionX));
	l.Position.y = std::stof(vector.at(e_PositionY));
	l.Position.z = std::stof(vector.at(e_PositionZ));
}


void SceneTextReader::applyToDirectionalLight(templateDirectionalLight& l, const std::vector<std::string>& vector)
{
	applyToLight(l, vector);

	enum objectInfo
	{
		e_DirectionX = 10,
		e_DirectionY = 11,
		e_DirectionZ = 12
	};

	l.Direction.x = std::stof(vector.at(e_DirectionX));
	l.Direction.y = std::stof(vector.at(e_DirectionY));
	l.Direction.z = std::stof(vector.at(e_DirectionZ));
}


void SceneTextReader::applyToSpotLight(templateSpotLight& l, const std::vector<std::string>& vector)
{
	applyToLight(l, vector);

	enum objectInfo
	{
		e_PositionX = 10,
		e_PositionY = 11,
		e_PositionZ = 12
	};

	l.Position.x = std::stof(vector.at(e_PositionX));
	l.Position.y = std::stof(vector.at(e_PositionY));
	l.Position.z = std::stof(vector.at(e_PositionZ));
}

void SceneTextReader::applyToModel(templateModel& o, const std::vector<std::string>& vector)
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

	o.objectType = vector.at(e_objectType);

	o.PosX = std::stof(vector.at(e_PosX));
	o.PosY = std::stof(vector.at(e_PosY));
	o.PosZ = std::stof(vector.at(e_PosZ));

	o.RotX = std::stof(vector.at(e_RotX));
	o.RotY = std::stof(vector.at(e_RotY));
	o.RotZ = std::stof(vector.at(e_RotZ));

	o.ScaleX = std::stof(vector.at(e_ScaleX));
	o.ScaleY = std::stof(vector.at(e_ScaleY));
	o.ScaleZ = std::stof(vector.at(e_ScaleZ));
}

void SceneTextReader::applyToModelLightingTemplate(templateModelLighting& o, const std::vector<std::string>& vector)
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
		e_emissionMap = 17
	};

	applyToModel(o, vector);

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";

	if (vector.at(e_diffuseMap) == "null")
	{
		o.diffuseMap = "null"; // Object will use no diffuse map
	}
	else
	{
		o.diffuseMap = "res/textures/" + vector.at(e_diffuseMap) + ".png";
	}

	if (vector.at(e_specularMap) == "null")
	{
		o.specularMap = "null"; // Object will use no specular map
	}
	else
	{
		o.specularMap = "res/textures/" + vector.at(e_specularMap) + ".png";
	}

	if (vector.at(e_normalMap) == "null")
	{
		o.normalMap = "null"; // Object will use no normal map
	}
	else
	{
		o.normalMap = "res/textures/" + vector.at(e_normalMap) + ".png";
	}

	o.normalMapNormalize = std::stof(vector.at(e_normalMapNormalize));

	if (vector.at(e_heightMap) == "null")
	{
		o.heightMap = "null"; // Object will use no height map
	}
	else
	{
		o.heightMap = "res/textures/" + vector.at(e_heightMap) + ".png";
	}

	o.heightMapHeight = std::stof(vector.at(e_heightMapHeight));

	if (vector.at(e_emissionMap) == "null")
	{
		o.emissionMap = "null"; // Object will use no emission map
	}
	else
	{
		o.emissionMap = "res/textures/" + vector.at(e_emissionMap) + ".png";
	}
}

void SceneTextReader::applyToModelBasicTemplate(templateModelBasic& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_lightToCopy = 11
	};

	applyToModel(o, vector);

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";

	o.lightToCopy = std::stoi(vector.at(e_lightToCopy));
}


void SceneTextReader::applyToModelTerrainTemplate(templateModelTerrain& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_Elevation = 10,

		e_TerrainTextureMap = 11,
		e_TerrainHeightMap = 12
	};

	applyToModel(o, vector);

	o.elevation = std::stof(vector.at(e_Elevation));

	o.terrainTextureMap = "res/textures/" + vector.at(e_TerrainTextureMap) + ".png";
	o.terrainHeightMap = "res/textures/" + vector.at(e_TerrainHeightMap) + ".png";
}

void SceneTextReader::applyToModelSpriteTemplate(templateModelSprite& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_Sprite = 10
	};

	applyToModel(o, vector);

	o.sprite = "res/textures/" + vector.at(e_Sprite) + ".png";
}

void SceneTextReader::applyToModelEnvironmentTemplate(templateModelEnvironment& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_reflection = 11,

		e_refraction = 12
	};

	applyToModel(o, vector);

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";

	o.reflection = std::stof(vector.at(e_reflection));

	o.refraction = std::stof(vector.at(e_refraction));
}

void SceneTextReader::applyToModelGeometryTemplate(templateModelGeometry& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_reflection = 11,

		e_refraction = 12
	};

	applyToModel(o, vector);

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";
}

void SceneTextReader::applyToModelSkyTemplate(templateModelSky& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_skyboxTexture = 10
	};

	applyToModel(o, vector);

	o.skyboxTexture = vector.at(e_skyboxTexture);

}
