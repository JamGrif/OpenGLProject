#include "Scene.h"

#include <iostream>
#include <string>
#include <thread>

#include "Camera.h"
#include "EngineStatics.h"
#include "Input.h"
#include "SceneTextReader.h"
#include "LightManager.h"

#include "PerformanceTimer.h"

#include "models/Model.h"

// ran from std::threads in .loadScene()
static void createTextureThread(){ TextureManager::readTexturesFromFile(); }
static void createMeshThread(){ MeshManager::readMeshesFromFile(); }
static void createShaderThread(){ ShaderManager::readShadersFromFile(); }
static void createCubemapThread(){ CubemapManager::readCubemapsFromFile(); }

Scene::Scene(const std::string& sceneName)
	:m_sceneName(sceneName), m_sceneCamera(nullptr), m_sceneLightManager(nullptr),
	m_materialLightMinZ(-5.0f), m_materialLightMaxZ(9.0f), m_materialLightMinX(-25.0f), m_materialLightMaxX(-13.0f), m_materialLightIncZ(true),
	m_materialLightIncX(true), m_normalLightMaxZ(8.0f), m_normalLightMinZ(23.0f), m_normalLightIncZ(true), m_lightR(0.0f), m_lightG(0.0f), m_lightB(0.0f)
{
	std::cout << "Scene Initialized" << std::endl;

	m_sceneModels.reserve(50);
}
 
Scene::~Scene()
{
	m_sceneModels.clear();

	m_sceneCamera = nullptr;
	EngineStatics::setCamera(nullptr);

	m_sceneLightManager = nullptr;
	EngineStatics::setLightManager(nullptr);

	/*
		By not deleting all the scenes assets, it allows for quicker scene change after the content has been initially loaded
	*/

	TextureManager::clearTextures();
	CubemapManager::clearCubemaps();
	MeshManager::clearMeshes();
	ShaderManager::clearShaders();

	std::cout << "Scene Destroyed" << std::endl;
}

/// <summary>
/// Initializes the scene objects and creates the scenes models
/// </summary>
bool Scene::loadScene()
{
	// Scene essentials - all scenes must contain these objects
	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();

	// Run scene reader, giving it the scene objects vector and light manager to fill out
	SceneTextReader txtReader(m_sceneName, m_sceneModels, m_sceneLightManager);

	if (!txtReader.getStatus()) // Ensure textfile was read correctly
	{
		// Scene failed to load
		return false;
	}

	PerformanceTimer t("AssetLoadingTime");

	// Create threads to read assets from their files
	std::thread textureLoadThread(createTextureThread);
	std::thread meshLoadThread(createMeshThread);
	std::thread shaderLoadThread(createShaderThread);
	std::thread cubemapLoadThread(createCubemapThread);
	
	textureLoadThread.join();
	meshLoadThread.join();
	shaderLoadThread.join();
	cubemapLoadThread.join();

	// Once the files have been read and saved into the objects, use them to create the OpenGL objects
	TextureManager::createTextures();
	CubemapManager::createCubemaps();
	MeshManager::createMeshes();
	ShaderManager::createShaders();

	return true;
}

/// <summary>
/// Updates scene models, scene lights and the camera
/// </summary>
void Scene::updateScene()
{
	// Update functions
	updateSceneLight();
	updateOnInput();

	// Draw second pass of all models
	for (std::shared_ptr<Model>& m : m_sceneModels)
	{
		m->setMatrixValues();
	}

	// Draw second pass of all models
	for (std::shared_ptr<Model>& m : m_sceneModels)
	{
		m->updateModel();
	}
}

/// <summary>
/// Draws the sceneModels by calling its second draw pass function
/// </summary>
void Scene::drawScene()
{
	// Draw second pass of all models
	for (std::shared_ptr<Model>& m : m_sceneModels)
	{
		//m->setMatrixValues();
		m->drawPassTwo();
	}
}

/// <summary>
/// Returns the scenes text file name
/// </summary>
/// <returns></returns>
const std::string& Scene::getSceneName()
{
	return m_sceneName;
}

/// <summary>
/// Checks for input and updates various game objects as a result of those inputs - called every scene update
/// </summary>
void Scene::updateOnInput()
{
	/*
		Update scene camera
	*/

	m_sceneCamera->Update();
}

/// <summary>
/// Alters the lights position or colour every frame 
/// </summary>
void Scene::updateSceneLight()
{
	// Function needs to be rewritten at some point as its current implementation isn't the best

	/*
		Light over material showcase
	*/

	if (m_sceneName == "res/scenes/FMPscene.txt")
	{
		PointLight* localFirstPointLight = m_sceneLightManager->getPointLight(1);
		PointLight* localSecondPointLight = m_sceneLightManager->getPointLight(2);
		PointLight* localThirdPointLight = m_sceneLightManager->getPointLight(3);

		if (m_materialLightIncZ)
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->Position.z += 0.075f;
				if (localFirstPointLight->Position.z >= m_materialLightMaxZ)
				{
					localFirstPointLight->Position.z = m_materialLightMaxZ;
					m_materialLightIncZ = false;
				}
			}
		}
		else
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->Position.z -= 0.075f;
				if (localFirstPointLight->Position.z <= m_materialLightMinZ)
				{
					localFirstPointLight->Position.z = m_materialLightMinZ;
					m_materialLightIncZ = true;
				}
			}
		}

		if (m_materialLightIncX)
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->Position.x += 0.1f;
				if (localFirstPointLight->Position.x >= m_materialLightMaxX)
				{
					localFirstPointLight->Position.x = m_materialLightMaxX;
					m_materialLightIncX = false;
				}
			}
		}
		else
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->Position.x -= 0.1f;
				if (localFirstPointLight->Position.x <= m_materialLightMinX)
				{
					localFirstPointLight->Position.x = m_materialLightMinX;
					m_materialLightIncX = true;
				}
			}
		}

		//Light showing normals
		if (m_normalLightIncZ)
		{
			if (localSecondPointLight)
			{
				localSecondPointLight->Position.z += 0.05f;
				if (localSecondPointLight->Position.z >= m_normalLightMaxZ)
				{
					localSecondPointLight->Position.z = m_normalLightMaxZ;
					m_normalLightIncZ = false;
				}
			}
		}
		else
		{
			if (localSecondPointLight)
			{
				localSecondPointLight->Position.z += 0.05f;
				if (localSecondPointLight->Position.z >= m_normalLightMinZ)
				{
					localSecondPointLight->Position.z = m_normalLightMinZ;
					m_normalLightIncZ = true;
				}
			}
		}

		/*
			Coloured Lighting
		*/

		m_lightR -= 0.001f;
		if (m_lightR <= 0.0f)
		{
			m_lightR = 1.0f;
		}
			
		m_lightG += 0.003f;
		if (m_lightG >= 1.0f)
		{
			m_lightG = 0.0f;
		}

		m_lightB += 0.002f;
		if (m_lightB >= 1.0f)
		{
			m_lightB = 0.0f;
		}
			
		if (localThirdPointLight)
		{
			localThirdPointLight->Ambient = glm::vec3(m_lightR, m_lightG, m_lightB);
			localThirdPointLight->Diffuse = glm::vec3(m_lightR, m_lightG, m_lightB);
			localThirdPointLight->Specular = glm::vec3(m_lightR, m_lightG, m_lightB);
		}
	}


	if (m_sceneName == "res/scenes/materialTest.txt")
	{
		/*
			Coloured Lighting
		*/

		PointLight* localPointLight = m_sceneLightManager->getPointLight(0);
	
		m_materialtestlightR -= 0.001f;
		if (m_materialtestlightR <= 0.0f)
			m_materialtestlightR = 1.0f;
	
		m_materialtestlightG += 0.003f;
		if (m_materialtestlightG >= 1.0f)
			m_materialtestlightG = 0.0f;
	
		m_materialtestlightB += 0.002f;
		if (m_materialtestlightB >= 1.0f)
			m_materialtestlightB = 0.0f;

		
		if (localPointLight)
		{
			localPointLight->Ambient = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			localPointLight->Diffuse = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			localPointLight->Specular = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
		}
	
		if (m_materialtestLightIncZ)
		{
			if (localPointLight)
			{
				localPointLight->Position.z += 0.075f;
				if (localPointLight->Position.z >= m_materialtestLightMaxZ)
				{
					localPointLight->Position.z = m_materialtestLightMaxZ;
					m_materialtestLightIncZ = false;
				}
			}
		
		}
		else
		{
			if (localPointLight)
			{
				localPointLight->Position.z -= 0.075f;
				if (localPointLight->Position.z <= m_materialtestLightMinZ)
				{
					localPointLight->Position.z = m_materialtestLightMinZ;
					m_materialtestLightIncZ = true;
				}
			}
		}
		
		
		if (m_materialtestLightIncX)
		{
			if (localPointLight)
			{
				localPointLight->Position.x += 0.1f;
				if (localPointLight->Position.x >= m_materialtestLightMaxX)
				{
					localPointLight->Position.x = m_materialtestLightMaxX;
					m_materialtestLightIncX = false;
				}
			}
		}
		else
		{
			if (localPointLight)
			{
				localPointLight->Position.x -= 0.1f;
				if (localPointLight->Position.x <= m_materialtestLightMinX)
				{
					localPointLight->Position.x = m_materialtestLightMinX;
					m_materialtestLightIncX = true;
				}
			}
		}
	}
}

/// <summary>
/// Creates the scene camera
/// </summary>
/// <param name="x">Starting X position of camera</param>
/// <param name="y">Starting Y position of camera</param>
/// <param name="z">Starting Z position of camera</param>
void Scene::addSceneCamera(float x, float y, float z)
{
	m_sceneCamera = std::make_shared<Camera>(glm::vec3{ x,y,z });
	EngineStatics::setCamera(m_sceneCamera);
}

/// <summary>
/// Creates the scene light manager
/// </summary>
void Scene::addSceneLightManager()
{
	m_sceneLightManager = std::make_shared<LightManager>();
	EngineStatics::setLightManager(m_sceneLightManager);
}
