#include "pch.h"
#include "Core/EngineStatics.h"

#include "Rendering/OpenGLWindow.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Renderer.h"

std::shared_ptr<OpenGLWindow> EngineStatics::m_appWindow		= nullptr;

std::shared_ptr<SceneCamera>		 EngineStatics::m_camera	= nullptr;

glm::mat4* EngineStatics::m_projectionMatrix					= nullptr;

std::shared_ptr<SceneLightManager> EngineStatics::m_lightManager	= nullptr;

std::shared_ptr<Renderer>	EngineStatics::m_renderer			= nullptr;


const std::shared_ptr<OpenGLWindow>& EngineStatics::getAppWindow()
{
	return m_appWindow;
}

void EngineStatics::setAppWindow(const std::shared_ptr<OpenGLWindow>& w)
{
	m_appWindow = w;
}

const std::shared_ptr<SceneCamera>&	EngineStatics::getCamera()
{
	return m_camera;
}

void EngineStatics::setCamera(const std::shared_ptr<SceneCamera>& c)
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

const std::shared_ptr<SceneLightManager>& EngineStatics::getLightManager()
{
	return m_lightManager;
}

void EngineStatics::setLightManager(const std::shared_ptr<SceneLightManager>& lm)
{
	m_lightManager = lm;
}

const std::shared_ptr<Renderer> EngineStatics::getRenderer()
{
	return m_renderer;
}

void EngineStatics::setRenderer(const std::shared_ptr<Renderer>& r)
{
	m_renderer = r;
}
