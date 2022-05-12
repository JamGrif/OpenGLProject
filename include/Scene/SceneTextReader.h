#pragma once

#include <glm\vec3.hpp> // glm::vec3()

// Forward Declarations
class BaseEntity;
class LightingEntity;
class SceneLightManager;

struct templateBaseEntity;
struct templateLightingEntity;
struct templateSkyEntity;
struct templateBasicEntity;
struct templateTerrainEntity;
struct templateEnvironmentEntity;
struct templateGeometryEntity;

// Class that reads from a scene text file, feeding the scene models vectors with created objects from the text file
class SceneTextReader
{
public:
	SceneTextReader(const std::string& filename, std::vector<std::shared_ptr<BaseEntity>>& sceneMeshes, std::vector<std::shared_ptr<LightingEntity>>& sceneLightingEntities, std::shared_ptr<SceneLightManager>& sceneLightManager);
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
		Entity Objects
	*/

	std::vector<templateLightingEntity>		completedLightingEntityObjects;		// LightingEntity
	std::vector<templateBasicEntity>		completedBasicEntityObjects;		// BasicEntity
	std::vector<templateTerrainEntity>		completedTerrainEntityObjects;		// TerrainEntity
	std::vector<templateEnvironmentEntity>	completedEnvironmentEntityObjects;	// EnvironmentEntity
	std::vector<templateGeometryEntity>		completedGeometryEntityObjects;		// GeometryEntity
	std::vector<templateSkyEntity>			completedSkyEntityObjects;			// SkyEntity

	inline void applyToBaseEntity(templateBaseEntity& o, const std::vector<std::string>& fullLine);

	inline bool applyToLightingEntityTemplate(templateLightingEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToBasicEntityTemplate(templateBasicEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToTerrainEntityTemplate(templateTerrainEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToEnvironmentEntityTemplate(templateEnvironmentEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToGeometryEntityTemplate(templateGeometryEntity& o, const std::vector<std::string>& fullLine);
	inline bool applyToSkyEntityTemplate(templateSkyEntity& o, const std::vector<std::string>& fullLine);
};
