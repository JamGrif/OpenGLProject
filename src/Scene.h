#pragma once

#include <vector>

#include "Camera.h"
#include "Framebuffer.h"

#include "ModelSky.h"

#include "SceneTextReader.h"

//Class that contains all the objects of a loaded level. Loops through them every frame, updating and drawing them
class Scene
{
public:
	Scene(const std::string& sceneName);
	~Scene();

	bool					loadScene();
	void					updateScene();

	const std::string&		getSceneName();

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

