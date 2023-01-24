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

#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/Manager/ResourceManager.h"


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

// Resets the static variables in Scene.cpp
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

	MeshManager::Get()->ClearAllResources();
	TextureManager::Get()->ClearAllResources();
	ShaderManager::Get()->ResetAllResources();
	CubemapManager::Get()->ClearAllResources();
	TheMaterialManager::Instance()->ClearAllMaterials();

	PRINT_INFO("SCENE->{0} has unloaded sucessfully", m_sceneName);
}

/// <summary>
/// Parses the scene file, creating all models and assets
/// </summary>
bool Scene::LoadScene()
{
	PRINT_INFO("SCENE->Attempting to load scene {0}", m_sceneName);

	// Create scene essentials
	m_sceneCamera = std::make_shared<SceneCamera>();
	m_sceneLightManager = std::make_shared<SceneLightManager>();

	// Parse the XML scene file to create scene models and resources
	SceneParser sceneParser;
	if (!sceneParser.ParseSceneFile(m_sceneName, m_sceneModels, m_sceneLightManager, &m_sceneSky))
		return false;

	// Set essential pointers
	m_sceneSky->SetCameraPointer(m_sceneCamera);

	TheMaterialManager::Instance()->SetAllMaterialScenePointers(m_sceneLightManager, m_sceneCamera);
	for (const auto& model : m_sceneModels)
	{
		model->SetModelPointers(m_sceneCamera.get());
	}


	PRINT_INFO("SCENE->{0} has loaded sucessfully", m_sceneName);
	return true;
}

/// <summary>
/// Updates scene models, scene lights and the camera
/// </summary>
void Scene::UpdateScene()
{
	UpdateSceneLight();

	m_sceneCamera->Update();

	for (const auto& m : m_sceneModels)
	{
		m->UpdateModel();
	}
}


/// <summary>
/// Draws the sceneModels by calling its first and second draw pass function
/// </summary>
void Scene::DrawScene()
{
	// Draw second pass of all models

	for (const auto& m : m_sceneModels)
	{
		m->DrawModel();
	}


	if (m_sceneSky)
	{
		m_sceneSky->DrawSky();
	}
		

	//PRINT_TRACE("{0} entities were drawn this frame!", entityDrawCount);
	//entityDrawCount = 0;
}

/// <summary>
/// Alters the lights position or colour every frame 
/// </summary>
void Scene::UpdateSceneLight()
{
	// Function needs to be rewritten at some point as its current implementation isn't the best

	/*
		Light over material showcase
	*/

	if (m_sceneName == "res/scenes/FMPscene.xml")
	{
		std::shared_ptr<PointLight> localFirstPointLight = m_sceneLightManager->GetPointLight(1).lock();
		std::shared_ptr<PointLight> localSecondPointLight = m_sceneLightManager->GetPointLight(2).lock();
		std::shared_ptr<PointLight> localThirdPointLight = m_sceneLightManager->GetPointLight(3).lock();

		if (m_materialLightIncZ)
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_position.z += 0.075f;
				if (localFirstPointLight->m_position.z >= m_materialLightMaxZ)
				{
					localFirstPointLight->m_position.z = m_materialLightMaxZ;
					m_materialLightIncZ = false;
				}
			}
		}
		else
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_position.z -= 0.075f;
				if (localFirstPointLight->m_position.z <= m_materialLightMinZ)
				{
					localFirstPointLight->m_position.z = m_materialLightMinZ;
					m_materialLightIncZ = true;
				}
			}
		}

		if (m_materialLightIncX)
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_position.x += 0.1f;
				if (localFirstPointLight->m_position.x >= m_materialLightMaxX)
				{
					localFirstPointLight->m_position.x = m_materialLightMaxX;
					m_materialLightIncX = false;
				}
			}
		}
		else
		{
			if (localFirstPointLight)
			{
				localFirstPointLight->m_position.x -= 0.1f;
				if (localFirstPointLight->m_position.x <= m_materialLightMinX)
				{
					localFirstPointLight->m_position.x = m_materialLightMinX;
					m_materialLightIncX = true;
				}
			}
		}

		//Light showing normals
		if (m_normalLightIncZ)
		{
			if (localSecondPointLight)
			{
				localSecondPointLight->m_position.z += 0.05f;
				if (localSecondPointLight->m_position.z >= m_normalLightMaxZ)
				{
					localSecondPointLight->m_position.z = m_normalLightMaxZ;
					m_normalLightIncZ = false;
				}
			}
		}
		else
		{
			if (localSecondPointLight)
			{
				localSecondPointLight->m_position.z += 0.05f;
				if (localSecondPointLight->m_position.z >= m_normalLightMinZ)
				{
					localSecondPointLight->m_position.z = m_normalLightMinZ;
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

		std::shared_ptr<PointLight> localPointLight = m_sceneLightManager->GetPointLight(0).lock();

		float dt = static_cast<float>(ApplicationClock::GetDeltaTime());
	
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
				localPointLight->m_position.z += 4.25f * dt;
				if (localPointLight->m_position.z >= m_materialtestLightMaxZ)
				{
					localPointLight->m_position.z = m_materialtestLightMaxZ;
					m_materialtestLightIncZ = false;
				}
			}
		
		}
		else
		{
			if (localPointLight)
			{
				localPointLight->m_position.z -= 4.25f * dt;
				if (localPointLight->m_position.z <= m_materialtestLightMinZ)
				{
					localPointLight->m_position.z = m_materialtestLightMinZ;
					m_materialtestLightIncZ = true;
				}
			}
		}
		
		
		if (m_materialtestLightIncX)
		{
			if (localPointLight)
			{
				localPointLight->m_position.x += 3.75f * dt;
				if (localPointLight->m_position.x >= m_materialtestLightMaxX)
				{
					localPointLight->m_position.x = m_materialtestLightMaxX;
					m_materialtestLightIncX = false;
				}
			}
		}
		else
		{
			if (localPointLight)
			{
				localPointLight->m_position.x -= 3.75f * dt;
				if (localPointLight->m_position.x <= m_materialtestLightMinX)
				{
					localPointLight->m_position.x = m_materialtestLightMinX;
					m_materialtestLightIncX = true;
				}
			}
		}
	}
}
