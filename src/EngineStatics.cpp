#include "EngineStatics.h"

OpenGLWindow* EngineStatics::m_appWindow = nullptr;

int EngineStatics::m_screenWidth = 0;

int EngineStatics::m_screenHeight = 0;

Camera* EngineStatics::m_camera = nullptr;

glm::mat4* EngineStatics::m_projectionMatrix = nullptr;

LightManager* EngineStatics::m_lightManager = nullptr;

double EngineStatics::m_deltaTime = 0;



OpenGLWindow* EngineStatics::getAppWindow()
{
	return m_appWindow;
}


void EngineStatics::setAppWindow(OpenGLWindow* w)
{
	m_appWindow = w;
}

int EngineStatics::getScreenWidth()
{
	return m_screenWidth;
}

void EngineStatics::setScreenWidth(int width)
{
	m_screenWidth = width;
}

int EngineStatics::getScreenHeight()
{
	return m_screenHeight;
}

void EngineStatics::setScreenHeight(int height)
{
	m_screenHeight = height;
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

double EngineStatics::getDeltaTime()
{
	return m_deltaTime;
}

void EngineStatics::setDeltaTime(double dt)
{
	m_deltaTime = dt;
}


