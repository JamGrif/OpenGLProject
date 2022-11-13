#include "pch.h"
#include "Scene/Scene.h"

#include <thread>
#include <algorithm>

#include "Core/EngineStatics.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneParser.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Model.h"
#include "Scene/SceneSky.h"

#include "Rendering/MaterialManager.h"
#include "Rendering/MeshManager.h"
#include "Rendering/TextureManager.h"


// Variables are used to move various lights in different scenes
// will get redone eventually
static float	m_materialLightMinZ;
static float	m_materialLightMaxZ;
static float	m_materialLightMinX;
static float	m_materialLightMaxX;
static bool		m_materialLightIncZ;
static bool		m_materialLightIncX;

static float	m_materialtestLightMinZ;
static float	m_materialtestLightMaxZ;
static float	m_materialtestLightMinX;
static float	m_materialtestLightMaxX;
static bool		m_materialtestLightIncZ;
static bool		m_materialtestLightIncX;
static float	m_materialtestlightR;
static float	m_materialtestlightG;
static float	m_materialtestlightB;

static float	m_normalLightMaxZ;
static float	m_normalLightMinZ;
static bool		m_normalLightIncZ;

static float	m_lightR;
static float	m_lightG;
static float	m_lightB;

// Resets the static variables above
void resetSceneValues()
{	
	// FMPscene.txt exclusive variables
	m_materialLightMinZ = -5;
	m_materialLightMaxZ = 9;
	m_materialLightMinX = -25;
	m_materialLightMaxX = -13;
	m_materialLightIncZ = true;
	m_materialLightIncX = true;

	// lightTest.txt exclusive variables
	m_materialtestLightMinZ = -9;
	m_materialtestLightMaxZ = 9;
	m_materialtestLightMinX = -9;
	m_materialtestLightMaxX = 9;
	m_materialtestLightIncZ = true;
	m_materialtestLightIncX = true;
	m_materialtestlightR = 0.0f;
	m_materialtestlightG = 0.0f;
	m_materialtestlightB = 0.0f;

	m_normalLightMaxZ = 8;
	m_normalLightMinZ = 23;
	m_normalLightIncZ = true;

	m_lightR = 0.0f;
	m_lightG = 0.0f;
	m_lightB = 0.0f;
}

// Counts the number of draw calls in a frame
int entityDrawCount = 0;

// ran from std::threads in .loadScene()
//static void createTextureThread(){ OpenGLTextureManager::readTexturesFromFile(); }
//static void createMeshThread(){ OpenGLMeshManager::readMeshesFromFile(); }
//static void createShaderThread(){ OpenGLShaderManager::readShadersFromFile(); }
//static void createCubemapThread(){ CubemapManager::readCubemapsFromFile(); }

Scene::Scene(const std::string& sceneName)
	:m_sceneName(sceneName), m_sceneCamera(nullptr), m_sceneLightManager(nullptr)
{
	//m_sceneEntities.reserve(50);
	resetSceneValues();
}
 
Scene::~Scene()
{
	//m_sceneEntities.clear();
	m_sceneLightingEntities.clear();

	m_sceneCamera = nullptr;
	EngineStatics::setCamera(nullptr);

	m_sceneLightManager = nullptr;
	EngineStatics::setLightManager(nullptr);

	delete m_sceneSky;

	/*
		By not deleting all the scenes assets, it allows for quicker scene change after the content has been initially loaded
	*/

	//OpenGLTextureManager::clearTextures();
	//CubemapManager::clearCubemaps();
	//OpenGLMeshManager::clearMeshes();
	//OpenGLShaderManager::clearShaders();

	TheMeshManager::Instance()->clearAllMeshes();
	TheMaterialManager::Instance()->clearAllMaterials();
	TheTextureManager::Instance()->clearAllTextures();
	TheTextureManager::Instance()->clearAllCubemaps();
	TheShaderManager::Instance()->clearAllShaders();

	PRINT_INFO("SCENE->{0} has unloaded sucessfully", m_sceneName);
}

/// <summary>
/// Initializes the scene objects and creates the scenes models
/// </summary>
bool Scene::loadScene()
{
	// Scene essentials - all scenes must contain these objects
	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();

	PRINT_INFO("SCENE->Attempting to load scene {0}", m_sceneName);
	PerformanceTimer t("Scene loading");

	// Run scene reader, giving it the scene objects vector and light manager to fill out
	SceneParser txtReader(m_sceneName, m_sceneLightingEntities, m_sceneLightManager, &m_sceneSky);


	if (!txtReader.getStatus()) // Ensure textfile was read correctly
	{
		// Scene failed to load
		return false;
	}

	// Create threads to read assets from their files
	//std::thread textureLoadThread(createTextureThread);
	//std::thread meshLoadThread(createMeshThread);
	//std::thread shaderLoadThread(createShaderThread);
	//std::thread cubemapLoadThread(createCubemapThread);
	
	//textureLoadThread.join();
	//meshLoadThread.join();
	//shaderLoadThread.join();
	//cubemapLoadThread.join();

	// Once the files have been read and saved into the objects, use them to create the OpenGL objects
	//OpenGLTextureManager::createTextures();
	//CubemapManager::createCubemaps();
	//OpenGLMeshManager::createMeshes();
	//OpenGLShaderManager::createShaders();


	for (auto& m : m_sceneLightingEntities)
	{
		m->initEntity();
	}

	//m_sceneSky->initEntity();

	t.stop();
	PRINT_INFO("SCENE->{0} has loaded sucessfully", m_sceneName);

	return true;
}

/// <summary>
/// Updates scene models, scene lights and the camera
/// </summary>
void Scene::updateScene()
{
	// Update functions
	updateSceneLight();

	m_sceneCamera->Update();



	for (auto& m : m_sceneLightingEntities)
	{
		m->setMatrixValues();
	}

	//


	for (auto& m : m_sceneLightingEntities)
	{
		m->updateEntity();
	}
}

void Scene::drawSceneFirstPass()
{
	// Draw first pass of all models
	

	for (auto& m : m_sceneLightingEntities)
	{
		m->drawPassOne();
	}
}

/// <summary>
/// Draws the sceneModels by calling its first and second draw pass function
/// </summary>
void Scene::drawSceneSecondPass()
{
	// Draw second pass of all models

	for (auto& m : m_sceneLightingEntities)
	{
		m->drawPassTwo();
	}


	if (m_sceneSky)
	{
		m_sceneSky->drawPassTwo();
	}
		

	//PRINT_TRACE("{0} entities were drawn this frame!", entityDrawCount);
	//entityDrawCount = 0;
}

/// <summary>
/// Returns the scenes text file name
/// </summary>
/// <returns></returns>
const std::string& Scene::getSceneName()
{
	return m_sceneName;
}

size_t Scene::getEntityNum()
{
	return m_sceneLightingEntities.size();
}


std::shared_ptr<Model> Scene::getEntityAtIndex(int index)
{
	return m_sceneLightingEntities.at(index);
}

//const std::vector<std::shared_ptr<BaseEntity>>& Scene::getSceneEntityVector()
//{
//	return m_sceneEntities;
//}

const std::shared_ptr<SceneLightManager>& Scene::getSceneLightManager()
{
	return m_sceneLightManager;
}


const std::shared_ptr<SceneCamera>& Scene::getSceneCamera()
{
	return m_sceneCamera;
}

void Scene::deleteLightingEntityFromVector(int index)
{
	m_sceneLightingEntities.erase(m_sceneLightingEntities.begin() + index);
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
		std::shared_ptr<PointLight> localFirstPointLight = m_sceneLightManager->getPointLight(1).lock();
		std::shared_ptr<PointLight> localSecondPointLight = m_sceneLightManager->getPointLight(2).lock();
		std::shared_ptr<PointLight> localThirdPointLight = m_sceneLightManager->getPointLight(3).lock();

		if (m_materialLightIncZ)
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_Position.z += 0.075f;
				if (localFirstPointLight->m_Position.z >= m_materialLightMaxZ)
				{
					localFirstPointLight->m_Position.z = m_materialLightMaxZ;
					m_materialLightIncZ = false;
				}
			}
		}
		else
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_Position.z -= 0.075f;
				if (localFirstPointLight->m_Position.z <= m_materialLightMinZ)
				{
					localFirstPointLight->m_Position.z = m_materialLightMinZ;
					m_materialLightIncZ = true;
				}
			}
		}

		if (m_materialLightIncX)
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_Position.x += 0.1f;
				if (localFirstPointLight->m_Position.x >= m_materialLightMaxX)
				{
					localFirstPointLight->m_Position.x = m_materialLightMaxX;
					m_materialLightIncX = false;
				}
			}
		}
		else
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_Position.x -= 0.1f;
				if (localFirstPointLight->m_Position.x <= m_materialLightMinX)
				{
					localFirstPointLight->m_Position.x = m_materialLightMinX;
					m_materialLightIncX = true;
				}
			}
		}

		//Light showing normals
		if (m_normalLightIncZ)
		{
			if (localSecondPointLight)
			{
				localSecondPointLight->m_Position.z += 0.05f;
				if (localSecondPointLight->m_Position.z >= m_normalLightMaxZ)
				{
					localSecondPointLight->m_Position.z = m_normalLightMaxZ;
					m_normalLightIncZ = false;
				}
			}
		}
		else
		{
			if (localSecondPointLight)
			{
				localSecondPointLight->m_Position.z += 0.05f;
				if (localSecondPointLight->m_Position.z >= m_normalLightMinZ)
				{
					localSecondPointLight->m_Position.z = m_normalLightMinZ;
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
			localThirdPointLight->m_Ambient = glm::vec3(m_lightR, m_lightG, m_lightB);
			localThirdPointLight->m_Diffuse = glm::vec3(m_lightR, m_lightG, m_lightB);
			localThirdPointLight->m_Specular = glm::vec3(m_lightR, m_lightG, m_lightB);
		}
	}


	if (m_sceneName == "res/scenes/materialTest.txt")
	{
		/*
			Coloured Lighting
		*/

		std::shared_ptr<PointLight> localPointLight = m_sceneLightManager->getPointLight(0).lock();

		float dt = static_cast<float>(ApplicationClock::getDeltaTime());
	
		m_materialtestlightR -= 0.1f * dt;
		if (m_materialtestlightR <= 0.0f)
			m_materialtestlightR = 1.0f;
	
		m_materialtestlightG += 0.3f * dt;
		if (m_materialtestlightG >= 1.0f)
			m_materialtestlightG = 0.0f;
	
		m_materialtestlightB += 0.2f * dt;
		if (m_materialtestlightB >= 1.0f)
			m_materialtestlightB = 0.0f;

		
		if (localPointLight)
		{
			localPointLight->m_Ambient = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			localPointLight->m_Diffuse = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			localPointLight->m_Specular = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
		}
	
		if (m_materialtestLightIncZ)
		{
			if (localPointLight)
			{
				localPointLight->m_Position.z += 4.25f * dt;
				if (localPointLight->m_Position.z >= m_materialtestLightMaxZ)
				{
					localPointLight->m_Position.z = m_materialtestLightMaxZ;
					m_materialtestLightIncZ = false;
				}
			}
		
		}
		else
		{
			if (localPointLight)
			{
				localPointLight->m_Position.z -= 4.25f * dt;
				if (localPointLight->m_Position.z <= m_materialtestLightMinZ)
				{
					localPointLight->m_Position.z = m_materialtestLightMinZ;
					m_materialtestLightIncZ = true;
				}
			}
		}
		
		
		if (m_materialtestLightIncX)
		{
			if (localPointLight)
			{
				localPointLight->m_Position.x += 3.75f * dt;
				if (localPointLight->m_Position.x >= m_materialtestLightMaxX)
				{
					localPointLight->m_Position.x = m_materialtestLightMaxX;
					m_materialtestLightIncX = false;
				}
			}
		}
		else
		{
			if (localPointLight)
			{
				localPointLight->m_Position.x -= 3.75f * dt;
				if (localPointLight->m_Position.x <= m_materialtestLightMinX)
				{
					localPointLight->m_Position.x = m_materialtestLightMinX;
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
	m_sceneCamera = std::make_shared<SceneCamera>(glm::vec3{ x,y,z });
	EngineStatics::setCamera(m_sceneCamera);
}

/// <summary>
/// Creates the scene light manager
/// </summary>
void Scene::addSceneLightManager()
{
	m_sceneLightManager = std::make_shared<SceneLightManager>();
	EngineStatics::setLightManager(m_sceneLightManager);
}
