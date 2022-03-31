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

	static const std::shared_ptr<OpenGLWindow>&			getAppWindow();
	static void										setAppWindow(const std::shared_ptr<OpenGLWindow>& w);

	static const std::shared_ptr<Camera>&					getCamera();
	static void										setCamera(const std::shared_ptr<Camera>& c);

	static glm::mat4*								getProjectionMatrix();
	static void										setProjectionMatrix(glm::mat4* pm);

	static const std::shared_ptr<LightManager>&			getLightManager();
	static void										setLightManager(const std::shared_ptr<LightManager>& lm);

	static const std::shared_ptr<GameTimer>&		getGameTimer();
	static void										setGameTimer(const std::shared_ptr<GameTimer>& gt);

private:

	static std::shared_ptr<OpenGLWindow>	m_appWindow;

	static std::shared_ptr<Camera>			m_camera;

	static glm::mat4*						m_projectionMatrix;

	static std::shared_ptr<LightManager>	m_lightManager;

	static std::shared_ptr<GameTimer>		m_gameTimer;

};

