#include "pch.h"
#include "Scene/Scene.h"

#include "Scene/SceneCamera.h"
#include "Scene/SceneParser.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Model.h"
#include "Scene/SceneSky.h"

#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/Manager/ResourceManager.h"

// Variables are used to move various lights in different scenes
// ------ Needs to be redone ------
static float	g_materialLightMinZ;
static float	g_materialLightMaxZ;
static float	g_materialLightMinX;
static float	g_materialLightMaxX;
static bool		g_materialLightIncZ;
static bool		g_materialLightIncX;
			
static float	g_materialtestLightMinZ;
static float	g_materialtestLightMaxZ;
static float	g_materialtestLightMinX;
static float	g_materialtestLightMaxX;
static bool		g_materialtestLightIncZ;
static bool		g_materialtestLightIncX;
static float	g_materialtestlightR;
static float	g_materialtestlightG;
static float	g_materialtestlightB;
			
static float	g_normalLightMaxZ;
static float	g_normalLightMinZ;
static bool		g_normalLightIncZ;
			
static float	g_lightR;
static float	g_lightG;
static float	g_lightB;

// Resets the static variables in Scene.cpp
// ------Needs to be redone ------
static void resetSceneValues()
{	
	// FMPscene.txt exclusive variables
	g_materialLightMinZ = -5;
	g_materialLightMaxZ = 9;
	g_materialLightMinX = -25;
	g_materialLightMaxX = -13;
	g_materialLightIncZ = true;
	g_materialLightIncX = true;

	// lightTest.txt exclusive variables
	g_materialtestLightMinZ = -9;
	g_materialtestLightMaxZ = 9;
	g_materialtestLightMinX = -9;
	g_materialtestLightMaxX = 9;
	g_materialtestLightIncZ = true;
	g_materialtestLightIncX = true;
	g_materialtestlightR = 0.0f;
	g_materialtestlightG = 0.0f;
	g_materialtestlightB = 0.0f;

	g_normalLightMaxZ = 8;
	g_normalLightMinZ = 23;
	g_normalLightIncZ = true;

	g_lightR = 0.0f;
	g_lightG = 0.0f;
	g_lightB = 0.0f;
}

Scene::Scene(const std::string& sceneFilename)
	:m_sceneFilename(sceneFilename)
{
	//resetSceneValues();
}
 
Scene::~Scene()
{
	m_sceneModels.clear();

	MeshManager::Get()->ClearAllResources();
	TextureManager::Get()->ClearAllResources();
	ShaderManager::Get()->ResetAllResources();
	CubemapManager::Get()->ClearAllResources();

	TheMaterialManager::Instance()->ClearAllMaterials();

	PRINT_INFO("SCENE->{0} has unloaded sucessfully", m_sceneFilename);
}

/// <summary>
/// Parses the XML scene file, creating all models and assets
/// </summary>
bool Scene::LoadScene()
{
	PRINT_INFO("SCENE->Attempting to load scene {0}", m_sceneFilename);

	// Create scene essentials
	m_sceneCamera = std::make_shared<SceneCamera>();
	m_sceneLightManager = std::make_shared<SceneLightManager>();

	// Parse the XML scene file to create scene models and resources
	SceneParser sceneParser;
	if (!sceneParser.ParseSceneFile(m_sceneFilename, m_sceneModels, m_sceneLightManager, &m_sceneSky))
		return false;

	// Setup object pointers with scene data
	m_sceneSky->SetCameraPointer(m_sceneCamera);
	TheMaterialManager::Instance()->SetAllMaterialScenePointers(m_sceneLightManager, m_sceneCamera);

	for (const auto& model : m_sceneModels)
	{
		model->SetModelPointers(m_sceneCamera);
	}

	PRINT_INFO("SCENE->{0} has loaded sucessfully", m_sceneFilename);
	return true;
}

/// <summary>
/// Update scene models and camera
/// </summary>
void Scene::UpdateScene()
{
	//UpdateSceneLight();

	m_sceneCamera->Update();

	for (const auto& model : m_sceneModels)
	{
		model->UpdateModel();
	}
}

/// <summary>
/// Draw all scene models and the sky
/// </summary>
void Scene::RenderScene()
{
	for (const auto& m : m_sceneModels)
	{
		m->DrawModel();
	}

	if (m_sceneSky)
		m_sceneSky->DrawSky();
}

/// <summary>
/// Return the model at index within the SceneModels vector
/// </summary>
std::weak_ptr<Model> Scene::GetModelAtIndex(unsigned int index)
{
	if (index < m_sceneModels.size())
		return m_sceneModels[index];
	else
		return {};
}

/// <summary>
/// Delete a model at index within the SceneModels vector
/// </summary>
void Scene::DeleteModelAtIndex(unsigned int index)
{
	if (index < m_sceneModels.size())
		m_sceneModels.erase(m_sceneModels.begin() + index);
}

/// <summary>
/// Move the scene camera to specified position
/// </summary>
void Scene::SetSceneCameraPosition(Vector3D newPosition)
{
	m_sceneCamera->SetPosition(newPosition);
}

/// <summary>
/// Alters the lights position or colour every frame depending on which scene is loaded
/// ------ Need to be redone ------
/// </summary>
void Scene::UpdateSceneLight()
{
	//// Function needs to be rewritten at some point as its current implementation isn't the best
	//
	///*
	//	Light over material showcase
	//*/
	//
	//if (m_sceneFilename == "res/scenes/FMPscene.xml")
	//{
	//	std::shared_ptr<PointLight> localFirstPointLight = m_sceneLightManager->GetPointLight(1).lock();
	//	std::shared_ptr<PointLight> localSecondPointLight = m_sceneLightManager->GetPointLight(2).lock();
	//	std::shared_ptr<PointLight> localThirdPointLight = m_sceneLightManager->GetPointLight(3).lock();
	//
	//	if (g_materialLightIncZ)
	//	{
	//		if (localFirstPointLight)
	//		{
	//			localFirstPointLight->m_position.z += 0.075f;
	//			if (localFirstPointLight->m_position.z >= g_materialLightMaxZ)
	//			{
	//				localFirstPointLight->m_position.z = g_materialLightMaxZ;
	//				g_materialLightIncZ = false;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (localFirstPointLight)
	//		{
	//			localFirstPointLight->m_position.z -= 0.075f;
	//			if (localFirstPointLight->m_position.z <= g_materialLightMinZ)
	//			{
	//				localFirstPointLight->m_position.z = g_materialLightMinZ;
	//				g_materialLightIncZ = true;
	//			}
	//		}
	//	}
	//
	//	if (g_materialLightIncX)
	//	{
	//		if (localFirstPointLight)
	//		{
	//			localFirstPointLight->m_position.x += 0.1f;
	//			if (localFirstPointLight->m_position.x >= g_materialLightMaxX)
	//			{
	//				localFirstPointLight->m_position.x = g_materialLightMaxX;
	//				g_materialLightIncX = false;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (localFirstPointLight)
	//		{
	//			localFirstPointLight->m_position.x -= 0.1f;
	//			if (localFirstPointLight->m_position.x <= g_materialLightMinX)
	//			{
	//				localFirstPointLight->m_position.x = g_materialLightMinX;
	//				g_materialLightIncX = true;
	//			}
	//		}
	//	}
	//
	//	//Light showing normals
	//	if (g_normalLightIncZ)
	//	{
	//		if (localSecondPointLight)
	//		{
	//			localSecondPointLight->m_position.z += 0.05f;
	//			if (localSecondPointLight->m_position.z >= g_normalLightMaxZ)
	//			{
	//				localSecondPointLight->m_position.z = g_normalLightMaxZ;
	//				g_normalLightIncZ = false;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (localSecondPointLight)
	//		{
	//			localSecondPointLight->m_position.z += 0.05f;
	//			if (localSecondPointLight->m_position.z >= g_normalLightMinZ)
	//			{
	//				localSecondPointLight->m_position.z = g_normalLightMinZ;
	//				g_normalLightIncZ = true;
	//			}
	//		}
	//	}
	//
	//	/*
	//		Coloured Lighting
	//	*/
	//
	//	g_lightR -= 0.001f;
	//	if (g_lightR <= 0.0f)
	//		g_lightR = 1.0f;
	//	
	//		
	//	g_lightG += 0.003f;
	//	if (g_lightG >= 1.0f)
	//		g_lightG = 0.0f;
	//	
	//
	//	g_lightB += 0.002f;
	//	if (g_lightB >= 1.0f)
	//		g_lightB = 0.0f;
	//	
	//		
	//	if (localThirdPointLight)
	//	{
	//		localThirdPointLight->m_ambient = glm::vec3(g_lightR, g_lightG, g_lightB);
	//		localThirdPointLight->m_diffuse = glm::vec3(g_lightR, g_lightG, g_lightB);
	//		localThirdPointLight->m_specular = glm::vec3(g_lightR, g_lightG, g_lightB);
	//	}
	//}
	//
	//
	//if (m_sceneFilename == "res/scenes/materialTest.xml")
	//{
	//	/*
	//		Coloured Lighting
	//	*/
	//
	//	std::shared_ptr<PointLight> localPointLight = m_sceneLightManager->GetPointLight(0).lock();
	//
	//	float dt = static_cast<float>(ApplicationClock::GetDeltaTime());
	//
	//	g_materialtestlightR -= 0.1f * dt;
	//	if (g_materialtestlightR <= 0.0f)
	//		g_materialtestlightR = 1.0f;
	//
	//	g_materialtestlightG += 0.3f * dt;
	//	if (g_materialtestlightG >= 1.0f)
	//		g_materialtestlightG = 0.0f;
	//
	//	g_materialtestlightB += 0.2f * dt;
	//	if (g_materialtestlightB >= 1.0f)
	//		g_materialtestlightB = 0.0f;
	//
	//	
	//	if (localPointLight)
	//	{
	//		localPointLight->m_ambient = glm::vec3(g_materialtestlightR, g_materialtestlightG, g_materialtestlightB);
	//		localPointLight->m_diffuse = glm::vec3(g_materialtestlightR, g_materialtestlightG, g_materialtestlightB);
	//		localPointLight->m_specular = glm::vec3(g_materialtestlightR, g_materialtestlightG, g_materialtestlightB);
	//	}
	//
	//	if (g_materialtestLightIncZ)
	//	{
	//		if (localPointLight)
	//		{
	//			localPointLight->m_position.z += 4.25f * dt;
	//			if (localPointLight->m_position.z >= g_materialtestLightMaxZ)
	//			{
	//				localPointLight->m_position.z = g_materialtestLightMaxZ;
	//				g_materialtestLightIncZ = false;
	//			}
	//		}
	//	
	//	}
	//	else
	//	{
	//		if (localPointLight)
	//		{
	//			localPointLight->m_position.z -= 4.25f * dt;
	//			if (localPointLight->m_position.z <= g_materialtestLightMinZ)
	//			{
	//				localPointLight->m_position.z = g_materialtestLightMinZ;
	//				g_materialtestLightIncZ = true;
	//			}
	//		}
	//	}
	//	
	//	
	//	if (g_materialtestLightIncX)
	//	{
	//		if (localPointLight)
	//		{
	//			localPointLight->m_position.x += 3.75f * dt;
	//			if (localPointLight->m_position.x >= g_materialtestLightMaxX)
	//			{
	//				localPointLight->m_position.x = g_materialtestLightMaxX;
	//				g_materialtestLightIncX = false;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (localPointLight)
	//		{
	//			localPointLight->m_position.x -= 3.75f * dt;
	//			if (localPointLight->m_position.x <= g_materialtestLightMinX)
	//			{
	//				localPointLight->m_position.x = g_materialtestLightMinX;
	//				g_materialtestLightIncX = true;
	//			}
	//		}
	//	}
	//}
}
