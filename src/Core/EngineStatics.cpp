#include "pch.h"
#include "Core/EngineStatics.h"

#include "Rendering/OpenGLWindow.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/OpenGLRenderer.h"


// .cpp file - Class Definition

std::shared_ptr<SceneCamera> EngineStatics::m_camera = nullptr;

glm::mat4* EngineStatics::m_projectionMatrix = nullptr;

std::shared_ptr<SceneLightManager> EngineStatics::m_lightManager = nullptr;

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
