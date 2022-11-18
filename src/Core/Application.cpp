#include "pch.h"
#include "Core/Application.h"

#include "Core/UI.h"
#include "Scene/Scene.h"
#include "Rendering/OpenGLWindow.h"

#include "Rendering/OpenGLRenderer.h"
#include "Core/InputHandler.h"

Application::Application()
	:m_UI(nullptr), m_loadedScene(nullptr)
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

		handleInput();
		updateApp();
		renderApp();
	}
}

void Application::handleInput()
{
	// Check if user wants to toggle UI visibility
	if (InputHandler::Instance()->getKeyPressedOnce(Keyboard::Q))
	{
		m_UI->toggleUI();
	}
}

void Application::updateApp()
{
	m_loadedScene->updateScene();

	if (m_UI->getUiVisible())
	{
		// Check if loaded scene needs to change
		if (m_UI->getSceneNum() != 0)
			setScene(m_UI->getSceneNum());
	}
}

void Application::renderApp()
{
	TheOpenGLRenderer::Instance()->startOfFrame();

	if (m_loadedScene)
		m_loadedScene->drawScene();

	if (m_UI->getUiVisible())
	{
		m_UI->renderUI();
	}

	TheOpenGLRenderer::Instance()->endOfFrame();
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
