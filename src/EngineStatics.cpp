#include "EngineStatics.h"

#include "OpenGLWindow.h"
#include "Camera.h"
#include "LightManager.h"
#include "GameTimer.h"

OpenGLWindow* EngineStatics::m_appWindow		= nullptr;

Camera* EngineStatics::m_camera					= nullptr;

glm::mat4* EngineStatics::m_projectionMatrix	= nullptr;

LightManager* EngineStatics::m_lightManager		= nullptr;

GameTimer* EngineStatics::m_gameTimer			= nullptr;

OpenGLWindow* EngineStatics::getAppWindow()
{
	return m_appWindow;
}

void EngineStatics::setAppWindow(OpenGLWindow* w)
{
	m_appWindow = w;
}

Camera* EngineStatics::getCamera()
{
	return m_camera;
}

void EngineStatics::setCamera(Camera* c)
{
	m_camera = c;
}

glm::mat4* EngineStatics::getProjectionMatrix()
{
	return m_projectionMatrix;
}

void EngineStatics::setProjectionMatrix(glm::mat4* pm)
{
	m_projectionMatrix = pm;
}

LightManager* EngineStatics::getLightManager()
{
	return m_lightManager;
}

void EngineStatics::setLightManager(LightManager* lm)
{
	m_lightManager = lm;
}

GameTimer* EngineStatics::getGameTimer()
{
	return m_gameTimer;
}

void EngineStatics::setGameTimer(GameTimer* gt)
{
	m_gameTimer = gt;
}
