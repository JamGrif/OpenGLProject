#pragma once


#include "LightManager.h"
#include "models/ModelBasic.h"
#include "models/ModelLighting.h"
#include "models/ModelEnvironment.h"
#include "models/ModelSprite.h"
#include "models/ModelTerrain.h"
#include "models/ModelGeometry.h"

//Class that reads from a scene text file, feeding the scene models vectors with created objects from the text file
class SceneTextReader
{
public:

	SceneTextReader(const std::string& filename, std::vector<Model*>& sceneMeshes, LightManager* sceneLightManager);
	~SceneTextReader();

	bool getStatus();

private:
	std::string m_filename;

	bool m_status;
private:

	/*
		Light Objects
	*/

	struct templateLight
	{
		std::string modelType;

		glm::vec3	Ambient;
		glm::vec3	Diffuse;
		glm::vec3	Specular;
		bool		lightActive = false;
	};
	struct templatePointLight : public templateLight
	{
		glm::vec3	Position;
	};
	struct templateDirectionalLight : public templateLight
	{
		glm::vec3	Direction;
	};
	struct templateSpotLight : public templateLight
	{
		glm::vec3	Position;
		glm::vec3	Direction;
	};

	std::vector<templatePointLight> completedPointLightObjects;
	std::vector<templateDirectionalLight> completedDirectionalLightObjects;
	std::vector<templateSpotLight> completedSpotLightObjects;

	void applyToLight(templateLight& l, const std::vector<std::string>& vector);

	void applyToPointLight(templatePointLight& l, const std::vector<std::string>& vector);
	void applyToDirectionalLight(templateDirectionalLight& l, const std::vector<std::string>& vector);
	void applyToSpotLight(templateSpotLight& l, const std::vector<std::string>& vector);

	/*
		Model Objects
	*/


	struct templateModel
	{
		std::string objectType = ""; // ModelLighting, ModelBasic, ModelTerrain etc...

		float PosX = 0.0f;
		float PosY = 0.0f;
		float PosZ = 0.0f;

		float RotX = 0.0f;
		float RotY = 0.0f;
		float RotZ = 0.0f;

		float ScaleX = 0.0f;
		float ScaleY = 0.0f;
		float ScaleZ = 0.0f;
	};
	struct templateModelLighting : public templateModel
	{
		std::string mesh = "";

		std::string diffuseMap = "";

		std::string specularMap = "";

		std::string normalMap = "";
		bool normalMapNormalize;

		std::string heightMap = "";
		float heightMapHeight = 0.0f;

		std::string emissionMap = "";
	};
	struct templateModelBasic : public templateModel
	{
		std::string mesh = "";

		int lightToCopy = -1;
	};
	struct templateModelTerrain : public templateModel
	{
		float elevation = 0.0f;
	};
	struct templateModelSprite : public templateModel
	{
		std::string sprite = "";
	};
	struct templateModelEnvironment : public templateModel
	{
		std::string mesh = "";

		bool reflection = false;

		bool refraction = false;
	};
	struct templateModelGeometry : public templateModel
	{
		std::string mesh = "";
	};


	std::vector<templateModelLighting> completedModelLightObjects;
	std::vector<templateModelBasic> completedModelBasicObjects;
	std::vector<templateModelTerrain> completedModelTerrainObjects;
	std::vector<templateModelSprite> completedModelSpriteObjects;
	std::vector<templateModelEnvironment> completedModelEnvironmentObjects;
	std::vector<templateModelGeometry> completedModelGeometryObjects;

	void applyToModel(templateModel& o, const std::vector<std::string>& vector);

	void applyToModelLightingTemplate(templateModelLighting& o, const std::vector<std::string>& vector);
	void applyToModelBasicTemplate(templateModelBasic& o, const std::vector<std::string>& vector);
	void applyToModelTerrainTemplate(templateModelTerrain& o, const std::vector<std::string>& vector);
	void applyToModelSpriteTemplate(templateModelSprite& o, const std::vector<std::string>& vector);
	void applyToModelEnvironmentTemplate(templateModelEnvironment& o, const std::vector<std::string>& vector);
	void applyToModelGeometryTemplate(templateModelGeometry& o, const std::vector<std::string>& vector);

};
