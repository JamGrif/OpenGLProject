#pragma once

#include <glm\vec3.hpp> // glm::vec3()

// Forward Declarations
class BaseEntity;
class SceneLightManager;

// Class that reads from a scene text file, feeding the scene models vectors with created objects from the text file
class SceneTextReader
{
public:
	SceneTextReader(const std::string& filename, std::vector<std::shared_ptr<BaseEntity>>& sceneMeshes, std::shared_ptr<SceneLightManager>& sceneLightManager);
	~SceneTextReader();

	bool getStatus();
private:
	bool m_status;

private:

	/*
		Light Objects
	*/

	struct templateLight
	{
		std::string modelType;

		glm::vec3	Ambient = {0.0f, 0.0f, 0.0f};
		glm::vec3	Diffuse = { 0.0f, 0.0f, 0.0f };
		glm::vec3	Specular = { 0.0f, 0.0f, 0.0f };
		bool		lightActive = false;
	};
	struct templatePointLight : public templateLight
	{
		glm::vec3	Position = { 0.0f, 0.0f, 0.0f };
	};
	struct templateDirectionalLight : public templateLight
	{
		glm::vec3	Direction = { 0.0f, 0.0f, 0.0f };
	};
	struct templateSpotLight : public templateLight
	{
		glm::vec3	Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3	Direction = { 0.0f, 0.0f, 0.0f };
	};

	std::vector<templatePointLight> completedPointLightObjects;
	std::vector<templateDirectionalLight> completedDirectionalLightObjects;
	std::vector<templateSpotLight> completedSpotLightObjects;

	inline void applyToLight(templateLight& l, const std::vector<std::string>& fullLine);

	inline bool applyToPointLight(templatePointLight& l, const std::vector<std::string>& fullLine);
	inline bool applyToDirectionalLight(templateDirectionalLight& l, const std::vector<std::string>& fullLine);
	inline bool applyToSpotLight(templateSpotLight& l, const std::vector<std::string>& fullLine);

	/*
		Model Objects
	*/

	struct templateBaseEntity
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
	struct templateLightingEntity : public templateBaseEntity
	{
		std::string mesh = "";

		std::string diffuseMap = "";

		std::string specularMap = "";

		std::string normalMap = "";
		bool normalMapNormalize = false;

		std::string heightMap = "";
		float heightMapHeight = 0.0f;

		std::string emissionMap = "";

		bool gravity = false;
	};
	struct templateBasicEntity : public templateBaseEntity
	{
		std::string mesh = "";

		int lightToCopy = -1;
	};
	struct templateTerrainEntity : public templateBaseEntity
	{
		float elevation = 0.0f;

		std::string terrainTextureMap = "";
		std::string terrainHeightMap = "";
	};
	struct templateEnvironmentEntity : public templateBaseEntity
	{
		std::string mesh = "";

		bool reflection = false;

		bool refraction = false;
	};
	struct templateGeometryEntity : public templateBaseEntity
	{
		std::string mesh = "";
	};
	struct templateSkyEntity : public templateBaseEntity
	{
		std::string skyboxTexture = "";
	};


	std::vector<templateLightingEntity>		completedModelLightObjects;			// ModelLighting
	std::vector<templateBasicEntity>			completedModelBasicObjects;			// ModelBasic
	std::vector<templateTerrainEntity>		completedModelTerrainObjects;		// ModelTerrain
	std::vector<templateEnvironmentEntity>	completedModelEnvironmentObjects;	// ModelEnvironment
	std::vector<templateGeometryEntity>		completedModelGeometryObjects;		// ModelGeometry
	std::vector<templateSkyEntity>			completedModelSkyObjects;			// ModelSky

	inline void applyToModel(templateBaseEntity& o, const std::vector<std::string>& fullLine);

	inline bool applyToModelLightingTemplate(templateLightingEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelBasicTemplate(templateBasicEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelTerrainTemplate(templateTerrainEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelEnvironmentTemplate(templateEnvironmentEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelGeometryTemplate(templateGeometryEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelSkyTemplate(templateSkyEntity& o, const std::vector<std::string>& fullLine);
};
