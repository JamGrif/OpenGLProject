#pragma once
#include <glm\glm.hpp>
#include "Camera.h"
#include "LightManager.h"

//Used to allow classes to easily access required values created in other parts of the program
class EngineStatics
{
public:

	static GLFWwindow*		getAppWindow() { return m_appWindow; }
	static void				setAppWindow(GLFWwindow* w) { m_appWindow = w; }

	static int				getScreenWidth() { return m_screenWidth; }
	static void				setScreenWidth(int width) { m_screenWidth = width; }

	static int				getScreenHeight() { return m_screenHeight; }
	static void				setScreenHeight(int height) { m_screenHeight = height; }

	static Camera*			getCamera() { return m_camera; }
	static void				setCamera(Camera* c) { m_camera = c; }
	
	static glm::mat4*		getProjectionMatrix() { return m_projectionMatrix; }
	static void				setProjectionMatrix(glm::mat4* pm) { m_projectionMatrix = pm; }

	static glm::mat4*		getLightSpaceMatrix() { return m_lightSpaceMatrix; }
	static void				setLightSpaceMatrix(glm::mat4* lsm) { m_lightSpaceMatrix = lsm; }

	static LightManager*	getLightManager() { return m_lightManager; }
	static void				setLightManager(LightManager* lm) { m_lightManager = lm; }

	static double			getDeltaTime() { return m_deltaTime; }
	static void				setDeltaTime(double dt) { m_deltaTime = dt; }


private:

	static GLFWwindow*		m_appWindow;

	static int				m_screenWidth;

	static int				m_screenHeight;

	static Camera*			m_camera;

	static glm::mat4*		m_projectionMatrix;

	static glm::mat4*		m_lightSpaceMatrix;

	static LightManager*	m_lightManager;

	static double			m_deltaTime;

};

