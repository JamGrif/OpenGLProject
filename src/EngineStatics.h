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

	static int				getScreenWidth();
	static void				setScreenWidth(int width);

	static int				getScreenHeight();
	static void				setScreenHeight(int height);

	static Camera*			getCamera();
	static void				setCamera(Camera* c);
	
	static glm::mat4*		getProjectionMatrix();
	static void				setProjectionMatrix(glm::mat4* pm);

	static LightManager*	getLightManager();
	static void				setLightManager(LightManager* lm);

	static double			getDeltaTime();
	static void				setDeltaTime(double dt);

	static GameTimer*		getGameTimer();
	static void				setGameTimer(GameTimer* gt);

private:

	static OpenGLWindow*	m_appWindow;

	static int				m_screenWidth;

	static int				m_screenHeight;

	static Camera*			m_camera;

	static glm::mat4*		m_projectionMatrix;

	static LightManager*	m_lightManager;

	static double			m_deltaTime;

	static GameTimer*		m_gameTimer;

};

