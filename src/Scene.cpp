#include "Scene.h"

#include "EngineStatics.h"

#include "Input.h"

#include <iostream>
#include <string>

Scene::Scene(const std::string& sceneName)
	:m_sceneName(sceneName), m_sceneCamera(nullptr), m_sceneLightManager(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr),
	m_materialLightMinZ(-5.0f), m_materialLightMaxZ(9.0f), m_materialLightMinX(-25.0f), m_materialLightMaxX(-13.0f), m_materialLightIncZ(true),
	m_materialLightIncX(true), m_normalLightMaxZ(8.0f), m_normalLightMinZ(23.0f), m_normalLightIncZ(true), m_lightR(0.0f), m_lightG(0.0f), m_lightB(0.0f)
{
	std::cout << "Scene Initialized" << std::endl;
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
	//TextureManager::clearTextures();
	//TextureManager::clearCubemaps();
	//MeshManager::clearMeshes();
	//ShaderManager::clearShaders();

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
		Create scene reader object
	*/
	
	SceneTextReader* txtReader = new SceneTextReader(m_sceneName);
	
	/*
		Run scene reader, giving it the scene objects vector to fill out
	*/

	txtReader->runSceneTextReader(m_sceneMeshes, m_sceneLightManager);

	/*
		Clean up object
	*/

	delete txtReader;
	txtReader = nullptr;
	

	
	return true;
}

/// <summary>
/// Updates scene objects and models
/// </summary>
void Scene::updateScene()
{
	//std::cout << "updateScene start" << std::endl;
	//Update functions
	//updateSceneLight();
	//updateOnInput();
	m_sceneCamera->Update(EngineStatics::getDeltaTime());

	//Bind MSAA for object drawing
	//m_sceneMSAAFrameBuffer->bindFramebuffer();

	//UNUSED code that was used for shadowing ---
	//Draw first pass of all models
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
	
	for (Model* m : m_sceneMeshes)
	{
		m->setMatrixValues();
		//m->drawPassOne(); //drawPassOne is unused as it was used for shadowing in the ModelLighting class
	}

	//UNUSED code that was used for shadowing ---
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, depthMap);

	//Draw second pass of all models
	for (Model* m : m_sceneMeshes)
	{
		m->drawPassTwo();
	}
	//std::cout << "updateScene end" << std::endl;
	
	//Reads from the MSAA buffer and writes it to the Filter buffer
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_sceneMSAAFrameBuffer->getFBO());
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_sceneFilterFramebuffer->getFBO());
	//glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//m_sceneMSAAFrameBuffer->unbindFramebuffer();

	//std::cout << "before scenefilterframebuffer" << std::endl;
	//Draw screen filter to buffer
	//m_sceneFilterFramebuffer->draw(); // aaaaaaaaaaaaaaaaaaaaa
	//std::cout << "after scenefilterframebuffer" << std::endl;
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
		Framebuffer filters inputs
	*/

	//if (Input::getKeyPressed(GLFW_KEY_1))
	//{
	//	m_sceneFilterFramebuffer->setFrameFilter(screen_Default);
	//}
	//
	//if (Input::getKeyPressed(GLFW_KEY_2))
	//{
	//	m_sceneFilterFramebuffer->setFrameFilter(screen_Inverse);
	//}
	//
	//if (Input::getKeyPressed(GLFW_KEY_3))
	//{
	//	m_sceneFilterFramebuffer->setFrameFilter(screen_Greyscale);
	//}
	//
	//if (Input::getKeyPressed(GLFW_KEY_4))
	//{
	//	m_sceneFilterFramebuffer->setFrameFilter(screen_EdgeDetection);
	//}
	//
	//if (Input::getKeyPressed(GLFW_KEY_5))
	//{
	//	m_sceneFilterFramebuffer->setFrameFilter(screen_Drugs);
	//}

	/*
		Toggle lights
	*/

	//if (Input::getKeyPressedOnce(GLFW_KEY_6))
	//{
	//	if (m_sceneLightManager->getDirectionalLight(0) != nullptr)
	//		m_sceneLightManager->getDirectionalLight(0)->toggleActive();
	//}
	//
	//if (Input::getKeyPressedOnce(GLFW_KEY_7))
	//{
	//	if (m_sceneLightManager->getSpotLight(0) != nullptr)
	//		m_sceneLightManager->getSpotLight(0)->toggleActive();
	//}
	//
	//if (Input::getKeyPressedOnce(GLFW_KEY_8))
	//{
	//	if (m_sceneLightManager->getPointLight(0) != nullptr)
	//			m_sceneLightManager->getPointLight(0)->toggleActive();
	//}
	//
	//if (Input::getKeyPressedOnce(GLFW_KEY_9))
	//{
	//	if (m_sceneLightManager->getPointLight(1) != nullptr)
	//		m_sceneLightManager->getPointLight(1)->toggleActive();
	//}
	//
	//if (Input::getKeyPressedOnce(GLFW_KEY_0))
	//{
	//	if (m_sceneLightManager->getPointLight(2) != nullptr)
	//		m_sceneLightManager->getPointLight(2)->toggleActive();
	//}
	//
	//if (Input::getKeyPressedOnce(GLFW_KEY_MINUS))
	//{
	//	if (m_sceneLightManager->getPointLight(3) != nullptr)
	//		m_sceneLightManager->getPointLight(3)->toggleActive();
	//}
}

/// <summary>
/// Alters the lights position or colour every frame 
/// </summary>
void Scene::updateSceneLight()
{
	/*
		Light over material showcase
	*/
	
	//if (m_materialLightIncZ)
	//{
	//	if (m_sceneLightManager->getPointLight(1) != nullptr)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.z += 0.075f;
	//		if (m_sceneLightManager->getPointLight(1)->Position.z >= m_materialLightMaxZ)
	//		{
	//			m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMaxZ;
	//			m_materialLightIncZ = false;
	//		}
	//	}
	//	
	//}
	//else
	//{
	//	if (m_sceneLightManager->getPointLight(1) != nullptr)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.z -= 0.075f;
	//		if (m_sceneLightManager->getPointLight(1)->Position.z <= m_materialLightMinZ)
	//		{
	//			m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMinZ;
	//			m_materialLightIncZ = true;
	//		}
	//	}
	//}
	//
	//if (m_materialLightIncX)
	//{
	//	if (m_sceneLightManager->getPointLight(1) != nullptr)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.x += 0.1f;
	//		if (m_sceneLightManager->getPointLight(1)->Position.x >= m_materialLightMaxX)
	//		{
	//			m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMaxX;
	//			m_materialLightIncX = false;
	//		}
	//	}
	//}
	//else
	//{
	//	if (m_sceneLightManager->getPointLight(1) != nullptr)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.x -= 0.1f;
	//		if (m_sceneLightManager->getPointLight(1)->Position.x <= m_materialLightMinX)
	//		{
	//			m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMinX;
	//			m_materialLightIncX = true;
	//		}
	//	}
	//}
	//
	//
	////Light showing normals
	//
	//
	//if (m_normalLightIncZ)
	//{
	//	if (m_sceneLightManager->getPointLight(2) != nullptr)
	//	{
	//		m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
	//		if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMaxZ)
	//		{
	//			m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMaxZ;
	//			m_normalLightIncZ = false;
	//		}
	//	}
	//}
	//else
	//{
	//	if (m_sceneLightManager->getPointLight(2) != nullptr)
	//	{
	//		m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
	//		if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMinZ)
	//		{
	//			m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMinZ;
	//			m_normalLightIncZ = true;
	//		}
	//	}
	//}
	//
	///*
	//	Coloured Lighting
	//*/
	//
	//m_lightR -= 0.001f;
	//if (m_lightR <= 0.0f)
	//	m_lightR = 1.0f;
	//
	//m_lightG += 0.003f;
	//if (m_lightG >= 1.0f)
	//	m_lightG = 0.0f;
	//
	//m_lightB += 0.002f;
	//if (m_lightB >= 1.0f)
	//	m_lightB = 0.0f;
	//
	//if (m_sceneLightManager->getPointLight(3) != nullptr)
	//{
	//	m_sceneLightManager->getPointLight(3)->Ambient = glm::vec3(m_lightR, m_lightG, m_lightB);
	//	m_sceneLightManager->getPointLight(3)->Diffuse = glm::vec3(m_lightR, m_lightG, m_lightB);
	//	m_sceneLightManager->getPointLight(3)->Specular = glm::vec3(m_lightR, m_lightG, m_lightB);
	//}
	
}

//UNUSED function used for shadowing - this was to setup the depthMap texture which the scene would be drawn too during the first pass of some models
//void Scene::setupShadowStuff()
//{
//
//	glGenFramebuffers(1, &depthMapFBO);
//	glGenTextures(1, &depthMap);
//	glBindTexture(GL_TEXTURE_2D, depthMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1280, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//
//	//Attach depth texture to framebuffers depth buffer
//	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//
//	//Tell framebuffer that no colour data will be rendered 
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	float near_plane = 1.0f, far_plane = 7.5f;
//	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
//	lightView = glm::lookAt(glm::vec3(-2.0f, 3.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	lightSpaceMatrix = lightProjection * lightView;
//
//	EngineStatics::setLightSpaceMatrix(&lightSpaceMatrix);
//	EngineStatics::setDepthMap(&depthMap);
//
//}

/// <summary>
/// Creates the scene camera, remakes it if one already exists
/// </summary>
/// <param name="x">Starting X position of camera</param>
/// <param name="y">Starting Y position of camera</param>
/// <param name="z">Starting Z position of camera</param>
void Scene::addSceneCamera(float x, float y, float z)
{
	if (m_sceneCamera != nullptr)
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
	if (m_sceneLightManager != nullptr)
	{
		delete m_sceneLightManager;
		m_sceneLightManager = nullptr;
	}

	m_sceneLightManager = new LightManager();
	EngineStatics::setLightManager(m_sceneLightManager);
}



