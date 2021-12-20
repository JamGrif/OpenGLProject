#include "Scene.h"

#include "EngineStatics.h"

#include "Input.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct templateModelLighting
{
	std::string modelType = ""; // ModelLighting, ModelBasic, ModelTerrain etc...

	float PosX = 0.0f;
	float PosY = 0.0f;
	float PosZ = 0.0f;

	float RotX = 0.0f;
	float RotY = 0.0f;
	float RotZ = 0.0f;

	float ScaleX = 0.0f;
	float ScaleY = 0.0f;
	float ScaleZ = 0.0f;

	std::string mesh = "";

	std::string diffuseMap = "";

	std::string specularMap = "";

	std::string normalMap = "";
	bool normalMapNormalize;

	std::string heightMap = "";
	float heightMapHeight = 0.0f;

	std::string emissionMap = "";
};

void applyToModelLightingTemplate(templateModelLighting& o, const std::vector<std::string>& vector);

std::vector<templateModelLighting> completedObjects;

Scene::Scene()
	:m_sceneCamera(nullptr), m_sceneLightManager(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr), m_mountainsX(nullptr), m_mountainsZ(nullptr),
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

	TextureManager::clearTextures();
	TextureManager::clearCubemaps();
	MeshManager::clearMeshes();
	ShaderManager::clearShaders();

	std::cout << "Scene Destroyed" << std::endl;
}

/// <summary>
/// Initializes the scene objects and creates the scenes models
/// </summary>
void Scene::initScene()
{
	// Scene essentials - all scenes must contain these objects

	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();
	m_sceneMeshes.push_back(new ModelSky()); // Skybox

	m_sceneFilterFramebuffer = new Framebuffer(false);
	m_sceneMSAAFrameBuffer = new Framebuffer(true);



	m_sceneLightManager->addDirectionalLight(-2.0f, -3.0f, -1.0f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.3f, 0.3f, 0.3f));

	std::ifstream fileStream("levelData.txt", std::ios::in);

	if (!fileStream)
	{
		std::cout << "File not found" << std::endl;
		exit(1);
	}

	std::string line = "";

	while (!fileStream.eof())
	{
		std::getline(fileStream, line);

		if (line == "") // Empty line, ignore and move to next line
		{
			//std::cout << "empty line detected" << std::endl;
			continue;
		}


		if (line.at(0) == '#') // Comment, ignore and move to next line
		{
			//std::cout << "# detected" << std::endl;
			continue;
		}

		templateModelLighting object;

		std::string buf;
		std::stringstream ss(line);

		std::vector<std::string> tokens; // Full line, vector of individual strings (words)

		/*
			Reads one line at a time and each line is one vector object and
			each collection of text separated by a space is its own string object
			Then the vector full of strings is sent to the applyToObject function where each word
			is applied to the corresponding attribute of the object

		*/


		while (ss >> buf)
		{
			tokens.push_back(buf);
		}

		applyToModelLightingTemplate(object, tokens);

		//std::cout << "modelType is " << object.modelType << std::endl;
		//
		//std::cout << "PosX is " << object.PosX << std::endl;
		//std::cout << "PosY is " << object.PosY << std::endl;
		//std::cout << "PosZ is " << object.PosZ << std::endl;
		//
		//std::cout << "RotX is " << object.RotX << std::endl;
		//std::cout << "RotY is " << object.RotY << std::endl;
		//std::cout << "RotZ is " << object.RotZ << std::endl;
		//
		//std::cout << "ScaleX is " << object.ScaleX << std::endl;
		//std::cout << "ScaleY is " << object.ScaleY << std::endl;
		//std::cout << "ScaleZ is " << object.ScaleZ << std::endl;
		//
		//std::cout << "mesh is " << object.mesh << std::endl;
		//
		//std::cout << "diffuseMap is " << object.diffuseMap << std::endl;
		//
		//std::cout << "specularMap is " << object.specularMap << std::endl;
		//
		//std::cout << "normalMap is " << object.normalMap << std::endl;
		//std::cout << "normalMapNormalize is " << object.normalMapNormalize << std::endl;
		//
		//std::cout << "heightMap is " << object.heightMap << std::endl;
		//std::cout << "heightMapHeight is " << object.heightMapHeight << std::endl;
		//
		//std::cout << "emissionMap is " << object.emissionMap << std::endl;
		//
		//std::cout << std::endl;

		completedObjects.push_back(object);

		

	}

	//std::cout << std::endl;
	//std::cout << "There are " << completedObjects.size() << " completed objects" << std::endl;


	for (int i = 0; i < completedObjects.size(); i++)
	{
		ModelLighting* model = new ModelLighting(glm::vec3(completedObjects.at(i).PosX, completedObjects.at(i).PosY, completedObjects.at(i).PosZ), glm::vec3(completedObjects.at(i).RotX, completedObjects.at(i).RotY, completedObjects.at(i).RotZ));
		model->SetXScale(completedObjects.at(i).ScaleX);
		model->SetYScale(completedObjects.at(i).ScaleY);
		model->SetZScale(completedObjects.at(i).ScaleZ);


		model->setMesh(completedObjects.at(i).mesh); 
		 

		if (completedObjects.at(i).diffuseMap != "null")
		{
			model->setDiffuseTexture(completedObjects.at(i).diffuseMap);
		}
		else
		{
			model->setDiffuseTexture("res/textures/blank.png"); // Models have to have a diffuse map
		}
		
		if (completedObjects.at(i).specularMap != "null")
		{
			model->setSpecularTexture(completedObjects.at(i).specularMap);
		}
		else
		{
			model->setSpecularTexture("res/textures/blank.png"); // Models have to have a specular map
		}

		if (completedObjects.at(i).normalMap != "null")
		{
			model->setNormalTexture(completedObjects.at(i).normalMap, completedObjects.at(i).normalMapNormalize);
		}

		
		if (completedObjects.at(i).heightMap != "null")
		{
			model->setHeightTexture(completedObjects.at(i).heightMap, completedObjects.at(i).heightMapHeight);
		}

		
		if (completedObjects.at(i).emissionMap != "null")
		{
			model->setEmissionTexture(completedObjects.at(i).emissionMap);
		}


		m_sceneMeshes.push_back(model);
	}

	




	////Terrain
	//m_sceneMeshes.push_back(new Terrain(glm::vec3(0.0f, 22.0f, 0.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(150,10,150), 3.0f)); //Surrounding house
	//
	//m_sceneMeshes.push_back(m_mountainsX = new Terrain(glm::vec3(125.0f, 20.0f, -30.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(50,10,200), -4.5f)); //Mountain on X axis
	//m_sceneMeshes.push_back(m_mountainsZ = new Terrain(glm::vec3(30.0f, 20.0f, -125.0f), glm::vec3(180.0f, 90.0f, 0.0f), glm::vec3(50, 10, 200), -4.5f)); //Mountain on Z axis
	//
	////Geometry shader object
	//ModelGeometry* g = new ModelGeometry(glm::vec3(-1.0f, -1.5f, 23.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//g->setMesh("res/meshes/barrel.obj");
	//g->SetXScale(1.5);
	//g->SetYScale(1.5);
	//g->SetZScale(1.5);
	//m_sceneMeshes.push_back(g);
	//
	////Reflection environment map object
	//ModelEnvironment* reflectionModel = new ModelEnvironment(glm::vec3(14.0f, 0.5f, -7.0f));
	//reflectionModel->toggleReflection(true);
	//reflectionModel->setMesh("res/meshes/heart.obj");
	//reflectionModel->SetXScale(1);
	//reflectionModel->SetYScale(1);
	//reflectionModel->SetZScale(1);
	//m_sceneMeshes.push_back(reflectionModel);
	//
	////Refraction environment map object
	//ModelEnvironment* refractionModel = new ModelEnvironment(glm::vec3(22.0f, 0.5f, -7.0f));
	//refractionModel->toggleRefraction(true);
	//refractionModel->setMesh("res/meshes/heart.obj");
	//refractionModel->SetXScale(1);
	//refractionModel->SetYScale(1);
	//refractionModel->SetZScale(1);
	//m_sceneMeshes.push_back(refractionModel);

	////Grass
	//std::vector<glm::vec3> grassPos =
	//{
	//	//Position
	//	glm::vec3(4.0f, -0.5f, 13.0f),
	//	glm::vec3(8.0f, -0.5f, 18.0f),
	//	glm::vec3(14.0f, -0.5f, 15.0f),
	//	glm::vec3(19.0f, -0.5f, -5.0f),
	//	glm::vec3(23.0f, -0.5f, 10.0f),
	//	glm::vec3(-4.0f, -0.5f,  13.0f),
	//	glm::vec3(-8.0f, -0.5f,  18.0f),
	//	glm::vec3(-14.0f, -0.5f, 15.0f),
	//	glm::vec3(-19.0f, -0.5f, 17.0f)
	//};
	//
	//for (int i = 0; i < grassPos.size(); i++)
	//{
	//	ModelSprite* grass = new ModelSprite(grassPos.at(i));
	//	grass->setSprite("res/textures/grass_sprite.png");
	//	grass->SetXScale(0.4f);
	//	grass->SetYScale(0.4f);
	//	grass->SetZScale(0.4f);
	//	m_sceneMeshes.push_back(grass);
	//}

	//Lighting
	
	//m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
	//m_sceneLightManager->getSpotLight(0)->toggleActive(); //Turns off spotlight by default
	//
	////Point lights
	//std::vector<glm::vec3> lightPosAmbDifSpc =
	//{
	//	//Position						//Ambient						//Diffuse						//Specular
	//	glm::vec3(0.0f, 3.0f, 0.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(0.4f, 0.4f, 0.4f),		//House light
	//	glm::vec3(-18.0f, 2.0f, 0.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),	//Material showcase light
	//	glm::vec3(18.0f, 2.0f, 14.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),	//Normal showcase light
	//	glm::vec3(-20.0f, 2.0f, 20.0f), glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),	//Coloured lighting
	//};
	//
	//for (int i = 0; i < lightPosAmbDifSpc.size(); i += 4)
	//{
	//	m_sceneLightManager->addPointLight(lightPosAmbDifSpc.at(i).x, lightPosAmbDifSpc.at(i).y, lightPosAmbDifSpc.at(i).z, lightPosAmbDifSpc.at(i + 1), lightPosAmbDifSpc.at(i + 2), lightPosAmbDifSpc.at(i + 3));
	//}
	//
	////Objects that copy lights
	//for (int i = 0; i < m_sceneLightManager->getCurrentPointLights(); i++)
	//{
	//	ModelBasic* light = new ModelBasic(lightPosAmbDifSpc.at(i));
	//	light->setMesh("res/meshes/cube.obj");
	//	light->copyPointLight(i);
	//	m_sceneMeshes.push_back(light);
	//}

	
	
}

/// <summary>
/// Updates scene objects and models
/// </summary>
void Scene::updateScene()
{
	//Update functions
	updateSceneLight();
	updateOnInput();
	m_sceneCamera->Update(EngineStatics::getDeltaTime());

	//Bind MSAA for object drawing
	m_sceneMSAAFrameBuffer->bindFramebuffer();

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

	//Reads from the MSAA buffer and writes it to the Filter buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_sceneMSAAFrameBuffer->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_sceneFilterFramebuffer->getFBO());
	glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	m_sceneMSAAFrameBuffer->unbindFramebuffer();
	
	//Draw screen filter to buffer
	m_sceneFilterFramebuffer->draw();

}

/// <summary>
/// Checks for input and updates various game objects as a result of those inputs - called every scene update
/// </summary>
void Scene::updateOnInput()
{
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
		Tessellation terrain inputs
	*/

	//if (Input::getKeyPressedOnce(GLFW_KEY_Q))
	//{
	//	m_mountainsX->alterElevation(0.5);
	//	m_mountainsZ->alterElevation(0.5);
	//}
	//
	//if (Input::getKeyPressedOnce(GLFW_KEY_E))
	//{
	//	m_mountainsX->alterElevation(-0.5);
	//	m_mountainsZ->alterElevation(-0.5);
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
	//	m_sceneLightManager->getPointLight(1)->Position.z += 0.075f;
	//	if (m_sceneLightManager->getPointLight(1)->Position.z >= m_materialLightMaxZ)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMaxZ;
	//		m_materialLightIncZ = false;
	//	}
	//}
	//else
	//{
	//	m_sceneLightManager->getPointLight(1)->Position.z -= 0.075f;
	//	if (m_sceneLightManager->getPointLight(1)->Position.z <= m_materialLightMinZ)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMinZ;
	//		m_materialLightIncZ = true;
	//	}
	//}
	//
	//if (m_materialLightIncX)
	//{
	//	m_sceneLightManager->getPointLight(1)->Position.x += 0.1f;
	//	if (m_sceneLightManager->getPointLight(1)->Position.x >= m_materialLightMaxX)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMaxX;
	//		m_materialLightIncX = false;
	//	}
	//}
	//else
	//{
	//	m_sceneLightManager->getPointLight(1)->Position.x -= 0.1f;
	//	if (m_sceneLightManager->getPointLight(1)->Position.x <= m_materialLightMinX)
	//	{
	//		m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMinX;
	//		m_materialLightIncX = true;
	//	}
	//}

	/*
		Light showing normals
	*/

	//if (m_normalLightIncZ)
	//{
	//	m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
	//	if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMaxZ)
	//	{
	//		m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMaxZ;
	//		m_normalLightIncZ = false;
	//	}
	//}
	//else
	//{
	//	m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
	//	if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMinZ)
	//	{
	//		m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMinZ;
	//		m_normalLightIncZ = true;
	//	}
	//}

	/*
		Coloured Lighting
	*/

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
	//m_sceneLightManager->getPointLight(3)->Ambient = glm::vec3(m_lightR,  m_lightG,  m_lightB);
	//m_sceneLightManager->getPointLight(3)->Diffuse = glm::vec3(m_lightR,  m_lightG,  m_lightB);
	//m_sceneLightManager->getPointLight(3)->Specular = glm::vec3(m_lightR, m_lightG,  m_lightB);
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


void applyToModelLightingTemplate(templateModelLighting& o, const std::vector<std::string>& vector)
{

	enum objectInfo
	{
		e_modelType = 0,
		e_PosX = 1,
		e_PosY = 2,
		e_PosZ = 3,

		e_RotX = 4,
		e_RotY = 5,
		e_RotZ = 6,

		e_ScaleX = 7,
		e_ScaleY = 8,
		e_ScaleZ = 9,

		e_mesh = 10,
		e_diffuseMap = 11,
		e_specularMap = 12,
		e_normalMap = 13,
		e_normalMapNormalize = 14,
		e_heightMap = 15,
		e_heightMapHeight = 16,
		e_emissionMap = 17
	};

	o.modelType = vector.at(e_modelType).c_str();

	o.PosX = std::stof(vector.at(e_PosX));
	o.PosY = std::stof(vector.at(e_PosY));
	o.PosZ = std::stof(vector.at(e_PosZ));

	o.RotX = std::stof(vector.at(e_RotX));
	o.RotY = std::stof(vector.at(e_RotY));
	o.RotZ = std::stof(vector.at(e_RotZ));

	o.ScaleX = std::stof(vector.at(e_ScaleX));
	o.ScaleY = std::stof(vector.at(e_ScaleY));
	o.ScaleZ = std::stof(vector.at(e_ScaleZ));

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";

	if (vector.at(e_diffuseMap) == "null")
	{
		o.diffuseMap = "null"; // Object will use no diffuse map
	}
	else
	{
		o.diffuseMap = "res/textures/" + vector.at(e_diffuseMap) + ".png";
	}

	if (vector.at(e_specularMap) == "null")
	{
		o.specularMap = "null"; // Object will use no specular map
	}
	else
	{
		o.specularMap = "res/textures/" + vector.at(e_specularMap) + ".png";
	}



	if (vector.at(e_normalMap) == "null")
	{
		o.normalMap = "null"; // Object will use no normal map
	}
	else
	{
		o.normalMap = "res/textures/" + vector.at(e_normalMap) + ".png";
	}

	o.normalMapNormalize = std::stof(vector.at(e_normalMapNormalize));



	if (vector.at(e_heightMap) == "null")
	{
		o.heightMap = "null"; // Object will use no height map
	}
	else
	{
		o.heightMap = "res/textures/" + vector.at(e_heightMap) + ".png";
	}

	o.heightMapHeight = std::stof(vector.at(e_heightMapHeight));

	if (vector.at(e_emissionMap) == "null")
	{
		o.emissionMap = "null"; // Object will use no emission map
	}
	else
	{
		o.emissionMap = "res/textures/" + vector.at(e_emissionMap) + ".png";
	}


}