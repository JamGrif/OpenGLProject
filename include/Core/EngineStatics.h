#pragma once

#include <glm/mat4x4.hpp>

// Forward Declarations
class OpenGLWindow;
class SceneCamera;
class SceneLightManager;
class Renderer;

// Used to allow classes to easily access required values created in other parts of the program
class EngineStatics
{
public:

	static const std::shared_ptr<OpenGLWindow>&	getAppWindow();
	static void									setAppWindow(const std::shared_ptr<OpenGLWindow>& w);

	static const std::shared_ptr<SceneCamera>&		getCamera();
	static void									setCamera(const std::shared_ptr<SceneCamera>& c);

	static glm::mat4*							getProjectionMatrix();
	static void									setProjectionMatrix(glm::mat4* pm);

	static const std::shared_ptr<SceneLightManager>&	getLightManager();
	static void									setLightManager(const std::shared_ptr<SceneLightManager>& lm);

	static const std::shared_ptr<Renderer>		getRenderer();
	static void									setRenderer(const std::shared_ptr<Renderer>& r);

private:

	static std::shared_ptr<OpenGLWindow>		m_appWindow;

	static std::shared_ptr<SceneCamera>				m_camera;

	static glm::mat4*							m_projectionMatrix;

	static std::shared_ptr<SceneLightManager>		m_lightManager;

	static std::shared_ptr<Renderer>			m_renderer;
};

