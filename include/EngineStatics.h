#pragma once

#include <glm\glm.hpp>

#include <memory>

// Forward Declarations
class OpenGLWindow;
class Camera;
class LightManager;
class GameTimer;

// Used to allow classes to easily access required values created in other parts of the program
class EngineStatics
{
public:

	static std::shared_ptr<OpenGLWindow>	getAppWindow();
	static void								setAppWindow(std::shared_ptr<OpenGLWindow> w);

	static std::shared_ptr<Camera>			getCamera();
	static void								setCamera(std::shared_ptr<Camera> c);
	
	static glm::mat4*						getProjectionMatrix();
	static void								setProjectionMatrix(glm::mat4* pm);

	static std::shared_ptr<LightManager>	getLightManager();
	static void								setLightManager(std::shared_ptr<LightManager> lm);

	static std::shared_ptr<GameTimer>		getGameTimer();
	static void								setGameTimer(std::shared_ptr<GameTimer> gt);

private:

	static std::shared_ptr<OpenGLWindow>	m_appWindow;

	static std::shared_ptr<Camera>			m_camera;

	static glm::mat4*						m_projectionMatrix;

	static std::shared_ptr<LightManager>	m_lightManager;

	static std::shared_ptr<GameTimer>		m_gameTimer;

};

