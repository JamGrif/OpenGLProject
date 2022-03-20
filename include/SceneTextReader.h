#pragma once

#include <glm\glm.hpp> // glm::vec3()

#include <vector>
#include <string>
#include <memory>

// Forward Declarations
class Model;
class LightManager;

// Class that reads from a scene text file, feeding the scene models vectors with created objects from the text file
class SceneTextReader
{
public:

	SceneTextReader(const std::string& filename, std::vector<std::shared_ptr<Model>>& sceneMeshes, std::shared_ptr<LightManager>& sceneLightManager);
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

	inline void applyToLight(templateLight& l, const std::vector<std::string>& fullLine);

	inline bool applyToPointLight(templatePointLight& l, const std::vector<std::string>& fullLine);
	inline bool applyToDirectionalLight(templateDirectionalLight& l, const std::vector<std::string>& fullLine);
	inline bool applyToSpotLight(templateSpotLight& l, const std::vector<std::string>& fullLine);

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
		bool normalMapNormalize = false;

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

		std::string terrainTextureMap = "";
		std::string terrainHeightMap = "";
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
	struct templateModelSky : public templateModel
	{
		std::string skyboxTexture = "";
	};


	std::vector<templateModelLighting>		completedModelLightObjects;			// ModelLighting
	std::vector<templateModelBasic>			completedModelBasicObjects;			// ModelBasic
	std::vector<templateModelTerrain>		completedModelTerrainObjects;		// ModelTerrain
	std::vector<templateModelSprite>		completedModelSpriteObjects;		// ModelSprite
	std::vector<templateModelEnvironment>	completedModelEnvironmentObjects;	// ModelEnvironment
	std::vector<templateModelGeometry>		completedModelGeometryObjects;		// ModelGeometry
	std::vector<templateModelSky>			completedModelSkyObjects;			// ModelSky

	inline void applyToModel(templateModel& o, const std::vector<std::string>& fullLine);

	inline bool applyToModelLightingTemplate(templateModelLighting& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelBasicTemplate(templateModelBasic& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelTerrainTemplate(templateModelTerrain& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelSpriteTemplate(templateModelSprite& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelEnvironmentTemplate(templateModelEnvironment& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelGeometryTemplate(templateModelGeometry& o, const std::vector<std::string>& fullLine);
	inline bool applyToModelSkyTemplate(templateModelSky& o, const std::vector<std::string>& fullLine);


	// thread stuff
private:
	void createLights(std::shared_ptr<LightManager> sceneLightManager, std::vector<templatePointLight> completedPointLightObjects, std::vector<templateDirectionalLight> completedDirectionalLightObjects, std::vector<templateSpotLight> completedSpotLightObjects);


};
