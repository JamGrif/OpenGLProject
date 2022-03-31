#include "pch.h"

#include "EngineStatics.h"

#include "OpenGLWindow.h"
#include "Camera.h"
#include "LightManager.h"
#include "GameTimer.h"

std::shared_ptr<OpenGLWindow> EngineStatics::m_appWindow	= nullptr;

std::shared_ptr<Camera>		 EngineStatics::m_camera		= nullptr;

glm::mat4* EngineStatics::m_projectionMatrix				= nullptr;

std::shared_ptr<LightManager> EngineStatics::m_lightManager	= nullptr;

std::shared_ptr<GameTimer> EngineStatics::m_gameTimer		= nullptr;


const std::shared_ptr<OpenGLWindow>& EngineStatics::getAppWindow()
{
	return m_appWindow;
}

void EngineStatics::setAppWindow(const std::shared_ptr<OpenGLWindow>& w)
{
	m_appWindow = w;
}

const std::shared_ptr<Camera>&	EngineStatics::getCamera()
{
	return m_camera;
}

void EngineStatics::setCamera(const std::shared_ptr<Camera>& c)
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

const std::shared_ptr<LightManager>& EngineStatics::getLightManager()
{
	return m_lightManager;
}

void EngineStatics::setLightManager(const std::shared_ptr<LightManager>& lm)
{
	m_lightManager = lm;
}

const std::shared_ptr<GameTimer>& EngineStatics::getGameTimer()
{
	return m_gameTimer;
}

void EngineStatics::setGameTimer(const std::shared_ptr<GameTimer>& gt)
{
	m_gameTimer = gt;
}
