#include "SceneTextReader.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

SceneTextReader::SceneTextReader(const std::string& filename)
{
	//std::cout << "SceneTextReader constructor" << std::endl;

	m_filename = filename;
}

SceneTextReader::~SceneTextReader()
{
	//std::cout << "SceneTextReader destructor" << std::endl;
}

bool SceneTextReader::runSceneTextReader(std::vector<Model*>& sceneMeshes, LightManager* sceneLightManager)
{
	//std::cout << "SceneTextReader runSceneTextReader" << std::endl;

	std::ifstream fileStream(m_filename, std::ios::in);

	if (!fileStream)
	{
		std::cout << "File not found" << std::endl;
		return false;
	}

	std::string line = "";
	int count = 0;
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);

		// Check if line is empty or a comment
		if (line == "" || line.at(0) == '#')
		{
			// Skip line
			continue;
		}


		std::string buf;
		std::stringstream ss(line);
		std::vector<std::string> textfileLine; // Full line, vector of individual strings (words)

		/*
			Reads one line at a time and each line is one vector object and
			each collection of text separated by a space is its own string object
			Then the vector full of strings is sent to the applyToObject function where each word
			is applied to the corresponding attribute of the object

		*/

		// Continuously read the textfile line, putting each word individually into the vector
		while (ss >> buf)
		{
			textfileLine.push_back(buf);
		}

		if (textfileLine.at(0) == "directionalLight")
		{
			templateDirectionalLight object;
			applyToDirectionalLight(object, textfileLine);
			completedDirectionalLightObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "pointLight")
		{
			templatePointLight object;
			applyToPointLight(object, textfileLine);
			completedPointLightObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "spotLight")
		{
			templateSpotLight object;
			applyToSpotLight(object, textfileLine);
			completedSpotLightObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelLighting") // .at(0) is the first word in the row - ObjectType
		{
			templateModelLighting object;
			applyToModelLightingTemplate(object, textfileLine);
			completedModelLightObjects.push_back(object);

		}
		else if (textfileLine.at(0) == "modelBasic")
		{
			templateModelBasic object;
			applyToModelBasicTemplate(object, textfileLine);
			completedModelBasicObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelTerrain")
		{
			templateModelTerrain object;
			applyToModelTerrainTemplate(object, textfileLine);
			completedModelTerrainObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelSprite")
		{
			templateModelSprite object;
			applyToModelSpriteTemplate(object, textfileLine);
			completedModelSpriteObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelEnvironment")
		{
			templateModelEnvironment object;
			applyToModelEnvironmentTemplate(object, textfileLine);
			completedModelEnvironmentObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelGeometry")
		{
			templateModelGeometry object;
			applyToModelGeometryTemplate(object, textfileLine);
			completedModelGeometryObjects.push_back(object);
		}
		else
		{
			std::cout << "Could not determine objectType - " << textfileLine.at(0) << " - FAILURE" << std::endl;
		}

	}// end of scene txt file read

	fileStream.close();

	//Now that the scene file has been read, actually create the objects and place into the scene meshes vector

	for (int i = 0; i < completedDirectionalLightObjects.size(); i++)
	{
		sceneLightManager->addDirectionalLight(
			completedDirectionalLightObjects.at(i).Ambient,
			completedDirectionalLightObjects.at(i).Diffuse,
			completedDirectionalLightObjects.at(i).Specular,
			completedDirectionalLightObjects.at(i).Direction
		);

	}

	for (int i = 0; i < completedPointLightObjects.size(); i++)
	{
		sceneLightManager->addPointLight(
			completedPointLightObjects.at(i).Ambient,
			completedPointLightObjects.at(i).Diffuse,
			completedPointLightObjects.at(i).Specular,
			completedPointLightObjects.at(i).Position
		);
	}

	for (int i = 0; i < completedSpotLightObjects.size(); i++)
	{
		sceneLightManager->addSpotLight(
			completedSpotLightObjects.at(i).Ambient,
			completedSpotLightObjects.at(i).Diffuse,
			completedSpotLightObjects.at(i).Specular,
			completedSpotLightObjects.at(i).Position
		);
	}

	for (int i = 0; i < completedModelLightObjects.size(); i++) // Create all modelLighting objects
	{
		ModelLighting* model = new ModelLighting();

		model->SetXPos(completedModelLightObjects.at(i).PosX);
		model->SetYPos(completedModelLightObjects.at(i).PosY);
		model->SetZPos(completedModelLightObjects.at(i).PosZ);

		model->SetXRot(completedModelLightObjects.at(i).RotX);
		model->SetYRot(completedModelLightObjects.at(i).RotY);
		model->SetZRot(completedModelLightObjects.at(i).RotZ);

		model->SetXScale(completedModelLightObjects.at(i).ScaleX);
		model->SetYScale(completedModelLightObjects.at(i).ScaleY);
		model->SetZScale(completedModelLightObjects.at(i).ScaleZ);

		model->setMesh(completedModelLightObjects.at(i).mesh);

		if (completedModelLightObjects.at(i).diffuseMap != "null")
		{
			model->setDiffuseTexture(completedModelLightObjects.at(i).diffuseMap);
		}
		else
		{
			model->setDiffuseTexture("res/textures/blank.png"); // Models have to have a diffuse map
		}

		if (completedModelLightObjects.at(i).specularMap != "null")
		{
			model->setSpecularTexture(completedModelLightObjects.at(i).specularMap);
		}
		else
		{
			model->setSpecularTexture("res/textures/blank.png"); // Models have to have a specular map
		}

		if (completedModelLightObjects.at(i).normalMap != "null")
		{
			model->setNormalTexture(completedModelLightObjects.at(i).normalMap, completedModelLightObjects.at(i).normalMapNormalize);
		}

		if (completedModelLightObjects.at(i).heightMap != "null")
		{
			model->setHeightTexture(completedModelLightObjects.at(i).heightMap, completedModelLightObjects.at(i).heightMapHeight);
		}

		if (completedModelLightObjects.at(i).emissionMap != "null")
		{
			model->setEmissionTexture(completedModelLightObjects.at(i).emissionMap);
		}

		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelBasicObjects.size(); i++)
	{
		ModelBasic* model = new ModelBasic();

		model->SetXPos(completedModelBasicObjects.at(i).PosX);
		model->SetYPos(completedModelBasicObjects.at(i).PosY);
		model->SetZPos(completedModelBasicObjects.at(i).PosZ);

		model->SetXRot(completedModelBasicObjects.at(i).RotX);
		model->SetYRot(completedModelBasicObjects.at(i).RotY);
		model->SetZRot(completedModelBasicObjects.at(i).RotZ);

		model->SetXScale(completedModelBasicObjects.at(i).ScaleX);
		model->SetYScale(completedModelBasicObjects.at(i).ScaleY);
		model->SetZScale(completedModelBasicObjects.at(i).ScaleZ);

		model->setMesh(completedModelBasicObjects.at(i).mesh);

		model->copyPointLight(completedModelBasicObjects.at(i).lightToCopy);

		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelTerrainObjects.size(); i++)
	{
		ModelTerrain* model = new ModelTerrain();

		model->SetXPos(completedModelTerrainObjects.at(i).PosX);
		model->SetYPos(completedModelTerrainObjects.at(i).PosY);
		model->SetZPos(completedModelTerrainObjects.at(i).PosZ);

		model->SetXRot(completedModelTerrainObjects.at(i).RotX);
		model->SetYRot(completedModelTerrainObjects.at(i).RotY);
		model->SetZRot(completedModelTerrainObjects.at(i).RotZ);

		model->SetXScale(completedModelTerrainObjects.at(i).ScaleX);
		model->SetYScale(completedModelTerrainObjects.at(i).ScaleY);
		model->SetZScale(completedModelTerrainObjects.at(i).ScaleZ);

		model->setElevation(completedModelTerrainObjects.at(i).elevation);

		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelSpriteObjects.size(); i++)
	{
		ModelSprite* model = new ModelSprite();

		model->SetXPos(completedModelSpriteObjects.at(i).PosX);
		model->SetYPos(completedModelSpriteObjects.at(i).PosY);
		model->SetZPos(completedModelSpriteObjects.at(i).PosZ);

		model->SetXRot(completedModelSpriteObjects.at(i).RotX);
		model->SetYRot(completedModelSpriteObjects.at(i).RotY);
		model->SetZRot(completedModelSpriteObjects.at(i).RotZ);

		model->SetXScale(completedModelSpriteObjects.at(i).ScaleX);
		model->SetYScale(completedModelSpriteObjects.at(i).ScaleY);
		model->SetZScale(completedModelSpriteObjects.at(i).ScaleZ);

		model->setSprite(completedModelSpriteObjects.at(i).sprite);
		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelEnvironmentObjects.size(); i++)
	{
		ModelEnvironment* model = new ModelEnvironment();

		model->SetXPos(completedModelEnvironmentObjects.at(i).PosX);
		model->SetYPos(completedModelEnvironmentObjects.at(i).PosY);
		model->SetZPos(completedModelEnvironmentObjects.at(i).PosZ);

		model->SetXRot(completedModelEnvironmentObjects.at(i).RotX);
		model->SetYRot(completedModelEnvironmentObjects.at(i).RotY);
		model->SetZRot(completedModelEnvironmentObjects.at(i).RotZ);

		model->SetXScale(completedModelEnvironmentObjects.at(i).ScaleX);
		model->SetYScale(completedModelEnvironmentObjects.at(i).ScaleY);
		model->SetZScale(completedModelEnvironmentObjects.at(i).ScaleZ);

		model->setMesh(completedModelEnvironmentObjects.at(i).mesh);

		model->toggleReflection(completedModelEnvironmentObjects.at(i).reflection);
		model->toggleRefraction(completedModelEnvironmentObjects.at(i).refraction);

		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelGeometryObjects.size(); i++)
	{
		ModelGeometry* model = new ModelGeometry();

		model->SetXPos(completedModelGeometryObjects.at(i).PosX);
		model->SetYPos(completedModelGeometryObjects.at(i).PosY);
		model->SetZPos(completedModelGeometryObjects.at(i).PosZ);

		model->SetXRot(completedModelGeometryObjects.at(i).RotX);
		model->SetYRot(completedModelGeometryObjects.at(i).RotY);
		model->SetZRot(completedModelGeometryObjects.at(i).RotZ);

		model->SetXScale(completedModelGeometryObjects.at(i).ScaleX);
		model->SetYScale(completedModelGeometryObjects.at(i).ScaleY);
		model->SetZScale(completedModelGeometryObjects.at(i).ScaleZ);

		model->setMesh(completedModelGeometryObjects.at(i).mesh);

		sceneMeshes.push_back(model);

	}

	return true;
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
		e_Elevation = 10
	};

	applyToModel(o, vector);

	o.elevation = std::stof(vector.at(e_Elevation));
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