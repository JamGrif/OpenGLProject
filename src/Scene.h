#pragma once

#include <vector>

#include "models/ModelSky.h"

// Forward Declarations
class Camera;
class Framebuffer;
class LightManager;

// Class that contains all the objects of a loaded level. Loops through them every frame, updating and drawing them
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

	/*
		FMPscene.txt exclusive variables
	*/

	// Light showing materials variables
	float					m_materialLightMinZ = -5;
	float					m_materialLightMaxZ = 9;
	float					m_materialLightMinX = -25;
	float					m_materialLightMaxX = -13;
	bool					m_materialLightIncZ = true;
	bool					m_materialLightIncX = true;

	/*
		lightTest.txt exclusive variables
	*/

	// Light showing materials variables
	float					m_materialtestLightMinZ = -9;
	float					m_materialtestLightMaxZ = 9;
	float					m_materialtestLightMinX = -9;
	float					m_materialtestLightMaxX = 9;
	bool					m_materialtestLightIncZ = true;
	bool					m_materialtestLightIncX = true;
	float					m_materialtestlightR = 0.0f;
	float					m_materialtestlightG = 0.0f;
	float					m_materialtestlightB = 0.0f;

	// Light showing normal maps variables
	float					m_normalLightMaxZ = 8;
	float					m_normalLightMinZ = 23;
	bool					m_normalLightIncZ = true;

	// Light showing coloured lighting variables
	float					m_lightR = 0.0f;
	float					m_lightG = 0.0f;
	float					m_lightB = 0.0f;

	// Cached screen size
	int m_cachedScreenWidth;
	int m_cachedScreenHeight;
};

