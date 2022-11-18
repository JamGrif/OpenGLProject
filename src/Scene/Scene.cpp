#include "pch.h"
#include "Scene/Scene.h"

#include <thread>
#include <algorithm>

#include "Scene/SceneCamera.h"
#include "Scene/SceneParser.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Model.h"
#include "Scene/SceneSky.h"

#include "Rendering/OpenGLRenderer.h"

#include "Rendering/MaterialManager.h"
#include "Rendering/MeshManager.h"
#include "Rendering/TextureManager.h"
#include "Rendering/ShaderManager.h"


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

Scene::Scene(const std::string& sceneName)
	:m_sceneName(sceneName), m_sceneCamera(nullptr), m_sceneLightManager(nullptr)
{
	resetSceneValues();
}
 
Scene::~Scene()
{
	m_sceneModels.clear();

	m_sceneCamera = nullptr;

	m_sceneLightManager = nullptr;

	delete m_sceneSky;


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
	addSceneCamera(Vector3D(0.0f, 2.0f, 0.0));
	addSceneLightManager();

	PRINT_INFO("SCENE->Attempting to load scene {0}", m_sceneName);
	//PerformanceTimer t("Scene loading");

	// Run scene reader, giving it the scene objects vector and light manager to fill out
	SceneParser txtReader(m_sceneName, m_sceneModels, m_sceneLightManager, &m_sceneSky);


	if (!txtReader.getStatus()) // Ensure textfile was read correctly
	{
		// Scene failed to load
		return false;
	}


	for (const auto& m : m_sceneModels)
	{
		m->setModelPointers(m_sceneCamera.get(), m_sceneLightManager.get());
	}

	TheMaterialManager::Instance()->setAllMaterialPointers(m_sceneLightManager.get(), m_sceneCamera.get(), TheOpenGLRenderer::Instance()->getProjectionMatrix());

	m_sceneSky->setCameraPointer(m_sceneCamera.get());

	//m_sceneSky->initEntity();

	//TheMaterialManager::Instance()->setAllMaterialPointers(m_sceneCamera.get());

	//t.stop();
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

	for (const auto& m : m_sceneModels)
	{
		m->setMatrixValues();
		m->updateModel();
	}
}


/// <summary>
/// Draws the sceneModels by calling its first and second draw pass function
/// </summary>
void Scene::drawScene()
{
	// Draw second pass of all models

	for (const auto& m : m_sceneModels)
	{
		m->drawModel();
	}


	if (m_sceneSky)
	{
		m_sceneSky->drawPassTwo();
	}
		

	//PRINT_TRACE("{0} entities were drawn this frame!", entityDrawCount);
	//entityDrawCount = 0;
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

	if (m_sceneName == "res/scenes/FMPscene.xml")
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
			localThirdPointLight->m_ambient = glm::vec3(m_lightR, m_lightG, m_lightB);
			localThirdPointLight->m_diffuse = glm::vec3(m_lightR, m_lightG, m_lightB);
			localThirdPointLight->m_specular = glm::vec3(m_lightR, m_lightG, m_lightB);
		}
	}


	if (m_sceneName == "res/scenes/materialTest.xml")
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
			localPointLight->m_ambient = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			localPointLight->m_diffuse = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			localPointLight->m_specular = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
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
void Scene::addSceneCamera(Vector3D position)
{
	m_sceneCamera = std::make_shared<SceneCamera>(glm::vec3{ position.getX(), position.getY(), position.getZ() });
}

/// <summary>
/// Creates the scene light manager
/// </summary>
void Scene::addSceneLightManager()
{
	m_sceneLightManager = std::make_shared<SceneLightManager>();
}
