#include "Scene.h"

#include "EngineStatics.h"

#include "Input.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

SceneTextReader::SceneTextReader(std::string filename)
{
	std::cout << "SceneTextReader constructor" << std::endl;
	
	m_filename = filename;

}

SceneTextReader::~SceneTextReader()
{
	std::cout << "SceneTextReader destructor" << std::endl;

}


bool SceneTextReader::runSceneTextReader(std::vector<Model*>& sceneMeshes)
{
	std::cout << "SceneTextReader runSceneTextReader" << std::endl;

	std::ifstream fileStream(m_filename, std::ios::in);

	if (!fileStream)
	{
		std::cout << "File not found" << std::endl;
		return false;
	}

	std::string line = "";
	int count = 0;
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);

		// Check if line is empty or a comment
		if (line == "" || line.at(0) == '#')
		{
			// Skip line
			continue;
		}


		std::string buf;
		std::stringstream ss(line);
		std::vector<std::string> textfileLine; // Full line, vector of individual strings (words)

		/*
			Reads one line at a time and each line is one vector object and
			each collection of text separated by a space is its own string object
			Then the vector full of strings is sent to the applyToObject function where each word
			is applied to the corresponding attribute of the object

		*/

		// Continuously read the textfile line, putting each word individually into the vector
		while (ss >> buf)
		{
			textfileLine.push_back(buf);
		}


		//count++;
		//std::cout << count << " number of times through loop" << std::endl;
		//std::cout << "tokens.at(0) is " << textfileLine.at(0) << std::endl;

		if (textfileLine.at(0) == "modelLighting") // .at(0) is the first word in the row - ModelType
		{
			//std::cout << "object is a modelLighting" << std::endl;
			templateModelLighting object;
			applyToModelLightingTemplate(object, textfileLine);
			completedModelLightObjects.push_back(object);

		}
		else if (textfileLine.at(0) == "modelBasic")
		{
			//std::cout << "object is a modelBasic" << std::endl;
			templateModelBasic object;
			applyToModelBasicTemplate(object, textfileLine);
			completedModelBasicObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelTerrain")
		{
			//std::cout << "object is a modelTerrain" << std::endl;
			templateModelTerrain object;
			applyToModelTerrainTemplate(object, textfileLine);
			completedModelTerrainObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelSprite")
		{
			//std::cout << "object is a modelSprite" << std::endl;
			templateModelSprite object;
			applyToModelSpriteTemplate(object, textfileLine);
			completedModelSpriteObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelEnvironment")
		{
			//std::cout << "object is a modelEnvironment" << std::endl;
			templateModelEnvironment object;
			applyToModelEnvironmentTemplate(object, textfileLine);
			completedModelEnvironmentObjects.push_back(object);
		}
		else if (textfileLine.at(0) == "modelGeometry")
		{
			//std::cout << "object is a modelGeometry" << std::endl;
			templateModelGeometry object;
			applyToModelGeometryTemplate(object, textfileLine);
			completedModelGeometryObjects.push_back(object);
		}
		else
		{
			std::cout << "Could not determine modelType - " << textfileLine.at(0) << " - FAILURE" << std::endl;
		}


		//std::cout << "moving to next line" << std::endl;

	}// end of scene txt file read

	fileStream.close();

	//Now that the scene file has been read, actually create the objects and place into the scene meshes vector

	for (int i = 0; i < completedModelLightObjects.size(); i++) // Create all modelLighting objects
	{
		ModelLighting* model = new ModelLighting();

		model->SetXPos(completedModelLightObjects.at(i).PosX);
		model->SetYPos(completedModelLightObjects.at(i).PosY);
		model->SetZPos(completedModelLightObjects.at(i).PosZ);

		model->SetXRot(completedModelLightObjects.at(i).RotX);
		model->SetYRot(completedModelLightObjects.at(i).RotY);
		model->SetZRot(completedModelLightObjects.at(i).RotZ);

		model->SetXScale(completedModelLightObjects.at(i).ScaleX);
		model->SetYScale(completedModelLightObjects.at(i).ScaleY);
		model->SetZScale(completedModelLightObjects.at(i).ScaleZ);

		model->setMesh(completedModelLightObjects.at(i).mesh);

		if (completedModelLightObjects.at(i).diffuseMap != "null")
		{
			model->setDiffuseTexture(completedModelLightObjects.at(i).diffuseMap);
		}
		else
		{
			model->setDiffuseTexture("res/textures/blank.png"); // Models have to have a diffuse map
		}

		if (completedModelLightObjects.at(i).specularMap != "null")
		{
			model->setSpecularTexture(completedModelLightObjects.at(i).specularMap);
		}
		else
		{
			model->setSpecularTexture("res/textures/blank.png"); // Models have to have a specular map
		}

		if (completedModelLightObjects.at(i).normalMap != "null")
		{
			model->setNormalTexture(completedModelLightObjects.at(i).normalMap, completedModelLightObjects.at(i).normalMapNormalize);
		}

		if (completedModelLightObjects.at(i).heightMap != "null")
		{
			model->setHeightTexture(completedModelLightObjects.at(i).heightMap, completedModelLightObjects.at(i).heightMapHeight);
		}

		if (completedModelLightObjects.at(i).emissionMap != "null")
		{
			model->setEmissionTexture(completedModelLightObjects.at(i).emissionMap);
		}

		sceneMeshes.push_back(model);

	}

	for (int i = 0; i < completedModelBasicObjects.size(); i++)
	{
		ModelBasic* model = new ModelBasic();

		model->SetXPos(completedModelBasicObjects.at(i).PosX);
		model->SetYPos(completedModelBasicObjects.at(i).PosY);
		model->SetZPos(completedModelBasicObjects.at(i).PosZ);

		model->SetXRot(completedModelBasicObjects.at(i).RotX);
		model->SetYRot(completedModelBasicObjects.at(i).RotY);
		model->SetZRot(completedModelBasicObjects.at(i).RotZ);

		model->SetXScale(completedModelBasicObjects.at(i).ScaleX);
		model->SetYScale(completedModelBasicObjects.at(i).ScaleY);
		model->SetZScale(completedModelBasicObjects.at(i).ScaleZ);

		model->setMesh(completedModelBasicObjects.at(i).mesh);

		model->copyPointLight(completedModelBasicObjects.at(i).lightToCopy);

		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelTerrainObjects.size(); i++)
	{
		ModelTerrain* model = new ModelTerrain();

		model->SetXPos(completedModelTerrainObjects.at(i).PosX);
		model->SetYPos(completedModelTerrainObjects.at(i).PosY);
		model->SetZPos(completedModelTerrainObjects.at(i).PosZ);

		model->SetXRot(completedModelTerrainObjects.at(i).RotX);
		model->SetYRot(completedModelTerrainObjects.at(i).RotY);
		model->SetZRot(completedModelTerrainObjects.at(i).RotZ);

		model->SetXScale(completedModelTerrainObjects.at(i).ScaleX);
		model->SetYScale(completedModelTerrainObjects.at(i).ScaleY);
		model->SetZScale(completedModelTerrainObjects.at(i).ScaleZ);

		model->setElevation(completedModelTerrainObjects.at(i).elevation);

		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelSpriteObjects.size(); i++)
	{
		ModelSprite* model = new ModelSprite();

		model->SetXPos(completedModelSpriteObjects.at(i).PosX);
		model->SetYPos(completedModelSpriteObjects.at(i).PosY);
		model->SetZPos(completedModelSpriteObjects.at(i).PosZ);

		model->SetXRot(completedModelSpriteObjects.at(i).RotX);
		model->SetYRot(completedModelSpriteObjects.at(i).RotY);
		model->SetZRot(completedModelSpriteObjects.at(i).RotZ);

		model->SetXScale(completedModelSpriteObjects.at(i).ScaleX);
		model->SetYScale(completedModelSpriteObjects.at(i).ScaleY);
		model->SetZScale(completedModelSpriteObjects.at(i).ScaleZ);

		model->setSprite(completedModelSpriteObjects.at(i).sprite);
		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelEnvironmentObjects.size(); i++)
	{
		ModelEnvironment* model = new ModelEnvironment();

		model->SetXPos(completedModelEnvironmentObjects.at(i).PosX);
		model->SetYPos(completedModelEnvironmentObjects.at(i).PosY);
		model->SetZPos(completedModelEnvironmentObjects.at(i).PosZ);

		model->SetXRot(completedModelEnvironmentObjects.at(i).RotX);
		model->SetYRot(completedModelEnvironmentObjects.at(i).RotY);
		model->SetZRot(completedModelEnvironmentObjects.at(i).RotZ);

		model->SetXScale(completedModelEnvironmentObjects.at(i).ScaleX);
		model->SetYScale(completedModelEnvironmentObjects.at(i).ScaleY);
		model->SetZScale(completedModelEnvironmentObjects.at(i).ScaleZ);

		model->setMesh(completedModelEnvironmentObjects.at(i).mesh);

		model->toggleReflection(completedModelEnvironmentObjects.at(i).reflection);
		model->toggleRefraction(completedModelEnvironmentObjects.at(i).refraction);

		sceneMeshes.push_back(model);
	}

	for (int i = 0; i < completedModelGeometryObjects.size(); i++)
	{
		ModelGeometry* model = new ModelGeometry();

		model->SetXPos(completedModelGeometryObjects.at(i).PosX);
		model->SetYPos(completedModelGeometryObjects.at(i).PosY);
		model->SetZPos(completedModelGeometryObjects.at(i).PosZ);

		model->SetXRot(completedModelGeometryObjects.at(i).RotX);
		model->SetYRot(completedModelGeometryObjects.at(i).RotY);
		model->SetZRot(completedModelGeometryObjects.at(i).RotZ);

		model->SetXScale(completedModelGeometryObjects.at(i).ScaleX);
		model->SetYScale(completedModelGeometryObjects.at(i).ScaleY);
		model->SetZScale(completedModelGeometryObjects.at(i).ScaleZ);

		model->setMesh(completedModelGeometryObjects.at(i).mesh);

		sceneMeshes.push_back(model);

	}

	return true;
}

void SceneTextReader::applyToModel(templateModel& o, const std::vector<std::string>& vector)
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
	};

	o.modelType = vector.at(e_modelType);

	o.PosX = std::stof(vector.at(e_PosX));
	o.PosY = std::stof(vector.at(e_PosY));
	o.PosZ = std::stof(vector.at(e_PosZ));

	o.RotX = std::stof(vector.at(e_RotX));
	o.RotY = std::stof(vector.at(e_RotY));
	o.RotZ = std::stof(vector.at(e_RotZ));

	o.ScaleX = std::stof(vector.at(e_ScaleX));
	o.ScaleY = std::stof(vector.at(e_ScaleY));
	o.ScaleZ = std::stof(vector.at(e_ScaleZ));
}

void SceneTextReader::applyToModelLightingTemplate(templateModelLighting& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_mesh = 10,
		e_diffuseMap = 11,
		e_specularMap = 12,
		e_normalMap = 13,
		e_normalMapNormalize = 14,
		e_heightMap = 15,
		e_heightMapHeight = 16,
		e_emissionMap = 17
	};

	applyToModel(o, vector);

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

void SceneTextReader::applyToModelBasicTemplate(templateModelBasic& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_lightToCopy = 11
	};

	applyToModel(o, vector);

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";

	o.lightToCopy = std::stof(vector.at(e_lightToCopy));
}


void SceneTextReader::applyToModelTerrainTemplate(templateModelTerrain& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_Elevation = 10
	};

	applyToModel(o, vector);

	o.elevation = std::stof(vector.at(e_Elevation));
}

void SceneTextReader::applyToModelSpriteTemplate(templateModelSprite& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_Sprite = 10
	};

	applyToModel(o, vector);

	o.sprite = "res/textures/" + vector.at(e_Sprite) + ".png";
}

void SceneTextReader::applyToModelEnvironmentTemplate(templateModelEnvironment& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_reflection = 11,

		e_refraction = 12
	};

	applyToModel(o, vector);

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";

	o.reflection = std::stof(vector.at(e_reflection));

	o.refraction = std::stof(vector.at(e_refraction));
}

void SceneTextReader::applyToModelGeometryTemplate(templateModelGeometry& o, const std::vector<std::string>& vector)
{
	enum objectInfo
	{
		e_mesh = 10,

		e_reflection = 11,

		e_refraction = 12
	};

	applyToModel(o, vector);

	o.mesh = "res/meshes/" + vector.at(e_mesh) + ".obj";
}















Scene::Scene(std::string sceneName)
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

	TextureManager::clearTextures();
	TextureManager::clearCubemaps();
	MeshManager::clearMeshes();
	ShaderManager::clearShaders();

	std::cout << "Scene Destroyed" << std::endl;
}

/// <summary>
/// Initializes the scene objects and creates the scenes models
/// </summary>
bool Scene::initScene()
{
	// Scene essentials - all scenes must contain these objects

	addSceneCamera(0.0f, 2.0f, 0.0f);
	addSceneLightManager();
	m_sceneMeshes.push_back(new ModelSky()); // Skybox

	m_sceneFilterFramebuffer = new Framebuffer(false);
	m_sceneMSAAFrameBuffer = new Framebuffer(true);

	/*
		Directional Light
	*/

	m_sceneLightManager->addDirectionalLight(-2.0f, -3.0f, -1.0f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.3f, 0.3f, 0.3f));

	/*
		Point Lights
	*/

	std::vector<glm::vec3> lightPosAmbDifSpc =
	{
		//Position						//Ambient						//Diffuse						//Specular
		glm::vec3(0.0f, 3.0f, 0.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec3(0.4f, 0.4f, 0.4f),	//House light
		glm::vec3(-18.0f, 2.0f, 0.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),		//Material showcase light
		glm::vec3(18.0f, 2.0f, 14.0f),	glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),		//Normal showcase light
		glm::vec3(-20.0f, 2.0f, 20.0f), glm::vec3(0.2f, 0.2f, 0.2f),	glm::vec3(1.0f, 1.0f, 1.0f),	glm::vec3(0.4f, 0.4f, 0.4f),		//Coloured lighting
	};
	
	for (int i = 0; i < lightPosAmbDifSpc.size(); i += 4)
	{
		m_sceneLightManager->addPointLight(lightPosAmbDifSpc.at(i).x, lightPosAmbDifSpc.at(i).y, lightPosAmbDifSpc.at(i).z, lightPosAmbDifSpc.at(i + 1), lightPosAmbDifSpc.at(i + 2), lightPosAmbDifSpc.at(i + 3));
	}

	/*
		Spot Light
	*/

	//m_sceneLightManager->addSpotLight(0.0f, 0.0f, 0.0f, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.8f,0.8f,0.8f), glm::vec3(1.0f, 1.0f, 1.0f));
	//m_sceneLightManager->getSpotLight(0)->toggleActive(); //Turns off spotlight by default

	/*
		Create scene reader object
	*/
	
	SceneTextReader* txtReader = new SceneTextReader(m_sceneName);

	/*
		Run scene reader, giving it the scene objects vector to fill out
	*/

	txtReader->runSceneTextReader(m_sceneMeshes);

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
		Toggle lights
	*/

	if (Input::getKeyPressedOnce(GLFW_KEY_6))
	{
		if (m_sceneLightManager->getDirectionalLight(0) != nullptr)
			m_sceneLightManager->getDirectionalLight(0)->toggleActive();
	}
	
	if (Input::getKeyPressedOnce(GLFW_KEY_7))
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
	
	if (m_materialLightIncZ)
	{
		m_sceneLightManager->getPointLight(1)->Position.z += 0.075f;
		if (m_sceneLightManager->getPointLight(1)->Position.z >= m_materialLightMaxZ)
		{
			m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMaxZ;
			m_materialLightIncZ = false;
		}
	}
	else
	{
		m_sceneLightManager->getPointLight(1)->Position.z -= 0.075f;
		if (m_sceneLightManager->getPointLight(1)->Position.z <= m_materialLightMinZ)
		{
			m_sceneLightManager->getPointLight(1)->Position.z = m_materialLightMinZ;
			m_materialLightIncZ = true;
		}
	}
	
	if (m_materialLightIncX)
	{
		m_sceneLightManager->getPointLight(1)->Position.x += 0.1f;
		if (m_sceneLightManager->getPointLight(1)->Position.x >= m_materialLightMaxX)
		{
			m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMaxX;
			m_materialLightIncX = false;
		}
	}
	else
	{
		m_sceneLightManager->getPointLight(1)->Position.x -= 0.1f;
		if (m_sceneLightManager->getPointLight(1)->Position.x <= m_materialLightMinX)
		{
			m_sceneLightManager->getPointLight(1)->Position.x = m_materialLightMinX;
			m_materialLightIncX = true;
		}
	}
	
	
	//Light showing normals
	
	
		if (m_normalLightIncZ)
		{
			m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
			if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMaxZ)
			{
				m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMaxZ;
				m_normalLightIncZ = false;
			}
		}
		else
		{
			m_sceneLightManager->getPointLight(2)->Position.z += 0.05f;
			if (m_sceneLightManager->getPointLight(2)->Position.z >= m_normalLightMinZ)
			{
				m_sceneLightManager->getPointLight(2)->Position.z = m_normalLightMinZ;
				m_normalLightIncZ = true;
			}
		}
	
	/*
		Coloured Lighting
	*/
	
	m_lightR -= 0.001f;
	if (m_lightR <= 0.0f)
		m_lightR = 1.0f;
	
	m_lightG += 0.003f;
	if (m_lightG >= 1.0f)
		m_lightG = 0.0f;
	
	m_lightB += 0.002f;
	if (m_lightB >= 1.0f)
		m_lightB = 0.0f;
	
	m_sceneLightManager->getPointLight(3)->Ambient = glm::vec3(m_lightR,  m_lightG,  m_lightB);
	m_sceneLightManager->getPointLight(3)->Diffuse = glm::vec3(m_lightR,  m_lightG,  m_lightB);
	m_sceneLightManager->getPointLight(3)->Specular = glm::vec3(m_lightR, m_lightG,  m_lightB);
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



