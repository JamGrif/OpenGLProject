#include "pch.h"
#include "Core/Application.h"

#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLFramebuffer.h"
#include "Core/EngineStatics.h"
#include "Scene/Scene.h"
#include "Core/UI.h"
#include "Rendering/Renderer.h"

Application::Application()
	:m_renderer(nullptr), m_UI(nullptr), m_loadedScene(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr)
{
}

Application::~Application()
{
	m_renderer = nullptr;
	EngineStatics::setRenderer(nullptr);
}

/// <summary>
/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
/// </summary>
/// <returns>Returns success or failure of initialization</returns>
bool Application::appInit()
{
	Log::init();

	m_renderer = std::make_shared<Renderer>();
	if (!m_renderer->getStatus())
	{
		PRINT_ERROR("Renderer failed to initialize");
		return false;
	}
	EngineStatics::setRenderer(m_renderer);

	/*
		Create applications objects
	*/

	// Create Game Timer object
	ApplicationClock::init();

	// Create Input object
	Input::init();

	// Create UI object
	m_UI = std::make_unique<UI>(true);

	// Create the apps Framebuffers
	m_sceneFilterFramebuffer = std::make_unique<OpenGLFramebuffer>(false);
	m_sceneMSAAFrameBuffer = std::make_unique<OpenGLFramebuffer>(true);

	// Create Scene object and set initial scene
	if (!setScene(e_shadowTest)) 
	{
		// Scene failed to load
		return false;
	}

	return true;
}

/// <summary>
/// Entire application loop of program
/// </summary>
void Application::appLoop()
{
	unsigned int windowWidth = EngineStatics::getAppWindow()->getWindowWidth();
	unsigned int windowHeight = EngineStatics::getAppWindow()->getWindowHeight();

	while (!EngineStatics::getAppWindow()->shouldClose())
	{
		ApplicationClock::tick();

		m_renderer->startOfFrame();

		if (m_UI)
		{
			m_UI->startOfFrame();

			if (m_UI->getUiVisible())
			{
				// Check if loaded scene needs to change
				if (m_UI->getSceneNum() != 0)
					setScene(m_UI->getSceneNum());

				// Check if applied screen filter needs to change
				if (m_UI->getFilterNum() != 0)
					setScreenFilter(m_UI->getFilterNum());
			}
		}
		
		if (m_loadedScene)
			m_loadedScene->updateScene();

		// Bind MSAA for object drawing
		if (m_sceneMSAAFrameBuffer)
			m_sceneMSAAFrameBuffer->bindFramebuffer();

		if (m_loadedScene)
			m_loadedScene->drawScene();

		// Reads from the MSAA buffer and writes it to the Filter buffer
		if (m_sceneMSAAFrameBuffer && m_sceneFilterFramebuffer)
		{
			m_sceneMSAAFrameBuffer->bindReadFramebuffer();
			m_sceneFilterFramebuffer->bindWriteFramebuffer();
			m_sceneMSAAFrameBuffer->copyToFramebuffer();
			m_sceneMSAAFrameBuffer->unbindFramebuffer();

			// Draw screen filter to buffer
			m_sceneFilterFramebuffer->draw();
		}

		if (m_UI)
			m_UI->drawInFrame();

		m_renderer->swapBuffers();
	}
}

/// <summary>
/// Function changes the scene to specified scene number, unloads the currently loaded scene and refreshes the UI light buttons
/// </summary>
/// <param name="newSceneNumber">Specified scene to change to (use sceneNames enum)</param>
/// <returns></returns>
bool Application::setScene(int newSceneNumber)
{
	std::string newSceneFilePath = "";
	switch (newSceneNumber)
	{
	case e_FMPscene:
		newSceneFilePath = "res/scenes/FMPscene.txt";
		break;
	case e_jamieTest:
		newSceneFilePath = "res/scenes/jamieTest.txt";
		break;
	case e_lightTest:
		newSceneFilePath = "res/scenes/lightTest.txt";
		break;
	case e_materialTest:
		newSceneFilePath = "res/scenes/materialTest.txt";
		break;
	case e_shadowTest:
		newSceneFilePath = "res/scenes/shadowTest.txt";
		break;
	default:
		PRINT_WARN("Specified sceneNumber is out of range:{0}", newSceneNumber);
		return false;
	}

	if (m_loadedScene != nullptr)
	{
		if (newSceneFilePath == m_loadedScene->getSceneName())
		{
			// Scene is already loaded
			return false;
		}
	}

	m_loadedScene = std::make_unique<Scene>(newSceneFilePath);

	if (m_loadedScene->loadScene())
	{
		// Scene successfully loaded
		if (m_UI)
			m_UI->refreshLightButtons();

		return true;
	}

	// Scene failed to load
	return false;
}

/// <summary>
/// Function changes the post-processing screen filter to the specified filter number
/// </summary>
/// <param name="newFilterNumber">Specified screen filter to change to (use screenFilters enum)</param>
void Application::setScreenFilter(int newFilterNumber)
{
	switch (newFilterNumber)
	{
	case 1:
		m_sceneFilterFramebuffer->setFrameFilter(screen_Default);
		break;
	case 2:
		m_sceneFilterFramebuffer->setFrameFilter(screen_Inverse);
		break;
	case 3:
		m_sceneFilterFramebuffer->setFrameFilter(screen_Greyscale);
		break;
	case 4:
		m_sceneFilterFramebuffer->setFrameFilter(screen_EdgeDetection);
		break;
	case 5:
		m_sceneFilterFramebuffer->setFrameFilter(screen_Drugs);
		break;
	default:
		PRINT_WARN("Specified filterNumber is out of range:{0}", newFilterNumber);
	};
}
