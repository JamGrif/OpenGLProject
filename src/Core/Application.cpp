#include "pch.h"
#include "Core/Application.h"

#include "Core/UI.h"
#include "Scene/Scene.h"
#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLFramebuffer.h"
#include "Rendering/OpenGLRenderer.h"
#include "Core/InputHandler.h"

Application::Application()
	:m_UI(nullptr), m_loadedScene(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr)
{
}

Application::~Application()
{
	TheOpenGLRenderer::Instance()->clean();

	m_loadedScene = nullptr;

	m_UI = nullptr;

	TheOpenGLWindow::Instance()->clean();
}

/// <summary>
/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
/// </summary>
/// <returns>Returns success or failure of initialization</returns>
bool Application::appInit()
{
	// Initialize logger
	Log::init();

	// Initialize OpenGL renderer
	TheOpenGLRenderer::Instance()->init();

	// Initialize the applications clock
	ApplicationClock::init();

	// Initialize input
	InputHandler::Instance()->init();

	// Initialize the UI
	m_UI = std::make_unique<UI>(false, m_loadedScene);

	// Create the apps Frame buffers
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
/// Entire loop of program
/// Called every frame
/// </summary>
void Application::appLoop()
{
	while (!TheOpenGLWindow::Instance()->shouldClose())
	{
		ApplicationClock::tick();

		//m_renderer->startOfFrame();
		TheOpenGLRenderer::Instance()->startOfFrame();
		
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

			m_UI->update();
		}

		TheOpenGLRenderer::Instance()->swapBuffers();
	}
}

/// <summary>
/// Function changes the scene to specified scene number, unloads the currently loaded scene and refreshes the UI light buttons
/// </summary>
/// <param name="newSceneNumber">Specified scene to change to (use sceneNames enum)</param>
bool Application::setScene(int newSceneNumber)
{
	std::string newSceneFilePath = "";
	switch (newSceneNumber)
	{
	case e_FMPscene:
		newSceneFilePath = "res/scenes/FMPscene.xml";
		break;
	case e_jamieTest:
		newSceneFilePath = "res/scenes/jamieTest.xml";
		break;
	case e_lightTest:
		newSceneFilePath = "res/scenes/lightTest.xml";
		break;
	case e_materialTest:
		newSceneFilePath = "res/scenes/materialTest.xml";
		break;
	case e_shadowTest:
		newSceneFilePath = "res/scenes/shadowTest.xml";
		break;
	default:
		PRINT_WARN("Specified sceneNumber is out of range:{0}", newSceneNumber);
		return false;
	}

	if (m_loadedScene)
	{
		if (newSceneFilePath == m_loadedScene->getSceneName())
		{
			// Scene is already loaded
			return false;
		}
	}

	m_loadedScene = nullptr;
	m_UI->updateSceneHandle(nullptr);
	m_loadedScene = std::make_shared<Scene>(newSceneFilePath);

	if (m_loadedScene->loadScene())
	{
		// Scene successfully loaded
		if (m_UI)
		{
			m_UI->updateSceneHandle(m_loadedScene);
		}

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
