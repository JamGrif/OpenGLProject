#pragma once

#include <vector>

#include "LightManager.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "ModelBasic.h"
#include "ModelLighting.h"
#include "ModelSky.h"
#include "ModelEnvironment.h"
#include "ModelSprite.h"
#include "ModelTerrain.h"
#include "ModelGeometry.h"

//Class that reads from a scene text file, feeding the scene models vectors with created objects from the text file
class SceneTextReader
{
public:

	SceneTextReader(const std::string& filename);
	~SceneTextReader();

	bool runSceneTextReader(std::vector<Model*>& sceneMeshes, LightManager* sceneLightManager);


	//void createModelLightingObject(templateModelLighting& o);

private:
	std::string m_filename;
private:

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

//Class that contains all the objects of a loaded level. Loops through them every frame, updating and drawing them
class Scene
{
public:
	Scene(std::string sceneName);
	~Scene();

	bool					initScene();
	void					updateScene();

private:

	void					addSceneCamera(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void					addSceneLightManager();

	void					updateOnInput();
	void					updateSceneLight();

	std::string				m_sceneName;

	std::vector<Model*>		m_sceneMeshes;

	Camera*					m_sceneCamera;

	LightManager*			m_sceneLightManager;

	Framebuffer*			m_sceneMSAAFrameBuffer;		//Scene is drawn to this buffer with MSAA applied
	Framebuffer*			m_sceneFilterFramebuffer;	//Recieves info from the MSAAframebuffer which then draws onto a quad which gets displayed to the screen

	//Terrain*				m_mountainsX;				//Adjustable mountain along X axis
	//Terrain*				m_mountainsZ;				//Adjustable mountain along Z axis

	//Light showing materials variables
	float					m_materialLightMinZ = -5;
	float					m_materialLightMaxZ = 9;
	float					m_materialLightMinX = -25;
	float					m_materialLightMaxX = -13;
	bool					m_materialLightIncZ = true;
	bool					m_materialLightIncX = true;

	//Light showing normal maps variables
	float					m_normalLightMaxZ = 8;
	float					m_normalLightMinZ = 23;
	bool					m_normalLightIncZ = true;

	//Light showing coloured lighting variables
	float					m_lightR = 0.0f;
	float					m_lightG = 0.0f;
	float					m_lightB = 0.0f;

	//UNUSED variables for shadowing
	//void					setupShadowStuff();
	 
	//unsigned int			depthMapFBO;
	//unsigned int			depthMap;
	//glm::mat4				lightProjection = glm::mat4(1.0f);
	//glm::mat4				lightView = glm::mat4(1.0f);
	//glm::mat4				lightSpaceMatrix = glm::mat4(1.0f);

};

