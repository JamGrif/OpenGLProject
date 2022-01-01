#include "Scene.h"

#include <iostream>
#include <string>

#include "EngineStatics.h"
#include "Input.h"
#include "SceneTextReader.h"

Scene::Scene(const std::string& sceneName)
	:m_sceneName(sceneName), m_sceneCamera(nullptr), m_sceneLightManager(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr),
	m_materialLightMinZ(-5.0f), m_materialLightMaxZ(9.0f), m_materialLightMinX(-25.0f), m_materialLightMaxX(-13.0f), m_materialLightIncZ(true),
	m_materialLightIncX(true), m_normalLightMaxZ(8.0f), m_normalLightMinZ(23.0f), m_normalLightIncZ(true), m_lightR(0.0f), m_lightG(0.0f), m_lightB(0.0f)
{
	std::cout << "Scene Initialized" << std::endl;

	m_cachedScreenWidth = EngineStatics::getScreenWidth();
	m_cachedScreenHeight = EngineStatics::getScreenHeight();
}
 
Scene::~Scene()
{
	for (Model* m : m_sceneMeshes)
	{
		delete m;
	}
	m_sceneMeshes.clear();

	EngineStatics::setCamera(nullptr);
	delete m_sceneCamera;
	m_sceneCamera = nullptr;

	EngineStatics::setLightManager(nullptr);
	delete m_sceneLightManager;
	m_sceneLightManager = nullptr;

	/*
		By not deleting all the scenes assets, it allows for quicker scene change after the content has been initially loaded
	*/
	TextureManager::clearTextures();
	TextureManager::clearCubemaps();
	MeshManager::clearMeshes();
	ShaderManager::clearShaders();

	delete m_sceneFilterFramebuffer;
	m_sceneFilterFramebuffer = nullptr;

	delete m_sceneMSAAFrameBuffer;
	m_sceneMSAAFrameBuffer = nullptr;

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
	m_sceneMeshes.push_back(new ModelSky()); // Skybox

	m_sceneFilterFramebuffer = new Framebuffer(false);
	m_sceneMSAAFrameBuffer = new Framebuffer(true);

	/*
		Run scene reader, giving it the scene objects vector and light manager to fill out
	*/

	SceneTextReader txtReader(m_sceneName, m_sceneMeshes, m_sceneLightManager);

	if (!txtReader.getStatus()) // Ensure textfile was read correctly
	{
		return false;
	}

	return true;
}

/// <summary>
/// Updates scene objects and models
/// </summary>
void Scene::updateScene()
{
	// Update functions
	updateSceneLight();
	updateOnInput();
	
	// Bind MSAA for object drawing
	m_sceneMSAAFrameBuffer->bindFramebuffer();

	// Draw second pass of all models
	for (Model* m : m_sceneMeshes)
	{
		m->setMatrixValues();
		m->drawPassTwo();
	}
	
	// Reads from the MSAA buffer and writes it to the Filter buffer
	m_sceneMSAAFrameBuffer->bindReadFramebuffer();
	m_sceneFilterFramebuffer->bindWriteFramebuffer();
	glBlitFramebuffer(0, 0, m_cachedScreenWidth, m_cachedScreenHeight, 0, 0, m_cachedScreenWidth, m_cachedScreenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	m_sceneMSAAFrameBuffer->unbindFramebuffer();

	// Draw screen filter to buffer
	m_sceneFilterFramebuffer->draw();
}

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

	m_sceneCamera->Update(EngineStatics::getDeltaTime());

	/*
		Framebuffer filters inputs
	*/

	if (Input::getKeyPressed(GLFW_KEY_1))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Default);
	}

	if (Input::getKeyPressed(GLFW_KEY_2))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Inverse);
	}

	if (Input::getKeyPressed(GLFW_KEY_3))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Greyscale);
	}

	if (Input::getKeyPressed(GLFW_KEY_4))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_EdgeDetection);
	}

	if (Input::getKeyPressed(GLFW_KEY_5))
	{
		m_sceneFilterFramebuffer->setFrameFilter(screen_Drugs);
	}

	/*
		Toggle lights
	*/

	if (Input::getKeyPressedOnce(GLFW_KEY_6))
	{
		if (m_sceneLightManager->getDirectionalLight(0) != nullptr)
			m_sceneLightManager->getDirectionalLight(0)->toggleActive();
	}
	
	if (Input::getKeyPressedOnce(GLFW_KEY_F))
	{
		if (m_sceneLightManager->getSpotLight(0) != nullptr)
			m_sceneLightManager->getSpotLight(0)->toggleActive();
	}
	
	if (Input::getKeyPressedOnce(GLFW_KEY_8))
	{
		if (m_sceneLightManager->getPointLight(0) != nullptr)
				m_sceneLightManager->getPointLight(0)->toggleActive();
	}
	
	if (Input::getKeyPressedOnce(GLFW_KEY_9))
	{
		if (m_sceneLightManager->getPointLight(1) != nullptr)
			m_sceneLightManager->getPointLight(1)->toggleActive();
	}
	
	if (Input::getKeyPressedOnce(GLFW_KEY_0))
	{
		if (m_sceneLightManager->getPointLight(2) != nullptr)
			m_sceneLightManager->getPointLight(2)->toggleActive();
	}
	
	if (Input::getKeyPressedOnce(GLFW_KEY_MINUS))
	{
		if (m_sceneLightManager->getPointLight(3) != nullptr)
			m_sceneLightManager->getPointLight(3)->toggleActive();
	}
}

/// <summary>
/// Alters the lights position or colour every frame 
/// </summary>
void Scene::updateSceneLight()
{
	/*
		Light over material showcase
	*/

	if (m_sceneName == "res/scenes/FMPscene.txt")
	{
		if (m_materialLightIncZ)
		{
			if (m_sceneLightManager->getPointLight(1) != nullptr)
			{
				m_sceneLightManager->getPointLight(1)->Position.z += 0.075f;
				if (m_sceneLightManager->getPointLight(1)->Position.z >= m_materialLightMaxZ)
				{
					m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMaxZ;
					m_materialLightIncZ = false;
				}
			}

		}
		else
		{
			if (m_sceneLightManager->getPointLight(1) != nullptr)
			{
				m_sceneLightManager->getPointLight(1)->Position.z -= 0.075f;
				if (m_sceneLightManager->getPointLight(1)->Position.z <= m_materialLightMinZ)
				{
					m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMinZ;
					m_materialLightIncZ = true;
				}
			}
		}


		if (m_materialLightIncX)
		{
			if (m_sceneLightManager->getPointLight(1) != nullptr)
			{
				m_sceneLightManager->getPointLight(1)->Position.x += 0.1f;
				if (m_sceneLightManager->getPointLight(1)->Position.x >= m_materialLightMaxX)
				{
					m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMaxX;
					m_materialLightIncX = false;
				}
			}
		}
		else
		{
			if (m_sceneLightManager->getPointLight(1) != nullptr)
			{
				m_sceneLightManager->getPointLight(1)->Position.x -= 0.1f;
				if (m_sceneLightManager->getPointLight(1)->Position.x <= m_materialLightMinX)
				{
					m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMinX;
					m_materialLightIncX = true;
				}
			}
		}


		//Light showing normals
		if (m_normalLightIncZ)
		{
			if (m_sceneLightManager->getPointLight(2) != nullptr)
			{
				m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
				if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMaxZ)
				{
					m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMaxZ;
					m_normalLightIncZ = false;
				}
			}
		}
		else
		{
			if (m_sceneLightManager->getPointLight(2) != nullptr)
			{
				m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
				if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMinZ)
				{
					m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMinZ;
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
			
		if (m_sceneLightManager->getPointLight(3) != nullptr)
		{
			m_sceneLightManager->getPointLight(3)->Ambient = glm::vec3(m_lightR, m_lightG, m_lightB);
			m_sceneLightManager->getPointLight(3)->Diffuse = glm::vec3(m_lightR, m_lightG, m_lightB);
			m_sceneLightManager->getPointLight(3)->Specular = glm::vec3(m_lightR, m_lightG, m_lightB);
		}
	}


	if (m_sceneName == "res/scenes/materialTest.txt")
	{
		/*
			Coloured Lighting
		*/

		m_materialtestlightR -= 0.001f;
		if (m_materialtestlightR <= 0.0f)
			m_materialtestlightR = 1.0f;

		m_materialtestlightG += 0.003f;
		if (m_materialtestlightG >= 1.0f)
			m_materialtestlightG = 0.0f;

		m_materialtestlightB += 0.002f;
		if (m_materialtestlightB >= 1.0f)
			m_materialtestlightB = 0.0f;

		if (m_sceneLightManager->getPointLight(0) != nullptr)
		{
			m_sceneLightManager->getPointLight(0)->Ambient = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			m_sceneLightManager->getPointLight(0)->Diffuse = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
			m_sceneLightManager->getPointLight(0)->Specular = glm::vec3(m_materialtestlightR, m_materialtestlightG, m_materialtestlightB);
		}

		if (m_materialtestLightIncZ)
		{
			if (m_sceneLightManager->getPointLight(0) != nullptr)
			{
				m_sceneLightManager->getPointLight(0)->Position.z += 0.075f;
				if (m_sceneLightManager->getPointLight(0)->Position.z >= m_materialtestLightMaxZ)
				{
					m_sceneLightManager->getPointLight(0)->Position.z = m_materialtestLightMaxZ;
					m_materialtestLightIncZ = false;
				}
			}
		
		}
		else
		{
			if (m_sceneLightManager->getPointLight(0) != nullptr)
			{
				m_sceneLightManager->getPointLight(0)->Position.z -= 0.075f;
				if (m_sceneLightManager->getPointLight(0)->Position.z <= m_materialtestLightMinZ)
				{
					m_sceneLightManager->getPointLight(0)->Position.z = m_materialtestLightMinZ;
					m_materialtestLightIncZ = true;
				}
			}
		}
		
		
		if (m_materialtestLightIncX)
		{
			if (m_sceneLightManager->getPointLight(0) != nullptr)
			{
				m_sceneLightManager->getPointLight(0)->Position.x += 0.1f;
				if (m_sceneLightManager->getPointLight(0)->Position.x >= m_materialtestLightMaxX)
				{
					m_sceneLightManager->getPointLight(0)->Position.x = m_materialtestLightMaxX;
					m_materialtestLightIncX = false;
				}
			}
		}
		else
		{
			if (m_sceneLightManager->getPointLight(0) != nullptr)
			{
				m_sceneLightManager->getPointLight(0)->Position.x -= 0.1f;
				if (m_sceneLightManager->getPointLight(0)->Position.x <= m_materialtestLightMinX)
				{
					m_sceneLightManager->getPointLight(0)->Position.x = m_materialtestLightMinX;
					m_materialtestLightIncX = true;
				}
			}
		}
	}

}

/// <summary>
/// Creates the scene camera, remakes it if one already exists
/// </summary>
/// <param name="x">Starting X position of camera</param>
/// <param name="y">Starting Y position of camera</param>
/// <param name="z">Starting Z position of camera</param>
void Scene::addSceneCamera(float x, float y, float z)
{
	if (m_sceneCamera != nullptr) // sceneCamera already exists
	{
		delete m_sceneCamera;
		m_sceneCamera = nullptr;
	}
	
	m_sceneCamera = new Camera(glm::vec3{ x,y,z });
	EngineStatics::setCamera(m_sceneCamera);
}

/// <summary>
/// Creates the scene light manager, remakes it if one already exists
/// </summary>
void Scene::addSceneLightManager()
{
	if (m_sceneLightManager != nullptr) // sceneLightmanger already exists
	{
		delete m_sceneLightManager;
		m_sceneLightManager = nullptr;
	}

	m_sceneLightManager = new LightManager();
	EngineStatics::setLightManager(m_sceneLightManager);
}



