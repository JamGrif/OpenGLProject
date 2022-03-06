#pragma once

#include <glm\glm.hpp>

// Forward Declarations
class OpenGLWindow;
class Camera;
class LightManager;
class GameTimer;

// Used to allow classes to easily access required values created in other parts of the program
class EngineStatics
{
public:

	static OpenGLWindow*	getAppWindow();
	static void				setAppWindow(OpenGLWindow* w);

	static Camera*			getCamera();
	static void				setCamera(Camera* c);
	
	static glm::mat4*		getProjectionMatrix();
	static void				setProjectionMatrix(glm::mat4* pm);

	static LightManager*	getLightManager();
	static void				setLightManager(LightManager* lm);

	static GameTimer*		getGameTimer();
	static void				setGameTimer(GameTimer* gt);

private:

	static OpenGLWindow*	m_appWindow;

	static Camera*			m_camera;

	static glm::mat4*		m_projectionMatrix;

	static LightManager*	m_lightManager;

	static GameTimer*		m_gameTimer;

};

