#include "pch.h"
#include "Core/Application.h"

#include "Core/InputHandler.h"
#include "Core/UI.h"
#include "Scene/Scene.h"
#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLRenderer.h"

Application::Application()
	:m_UI(nullptr), m_loadedScene(nullptr)
{
}

Application::~Application()
{
	TheOpenGLRenderer::Instance()->Clean();

	m_loadedScene = nullptr;

	m_UI = nullptr;

	TheOpenGLWindow::Instance()->Clean();
}

/// <summary>
/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
/// </summary>
bool Application::AppInit()
{
	// Initialize logger
	Log::Init();

	// Initialize OpenGL renderer
	TheOpenGLRenderer::Instance()->Init();

	// Initialize the applications clock
	ApplicationClock::Init();

	// Initialize input
	InputHandler::Instance()->Init();

	// Initialize the UI
	m_UI = std::make_unique<UI>(false, m_loadedScene);

	// Create Scene object and set initial scene
	if (!SetScene(e_jamieTest)) 
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
void Application::AppLoop()
{
	while (!TheOpenGLWindow::Instance()->ShouldClose())
	{
		ApplicationClock::Tick();

		HandleInput();
		UpdateApp();
		RenderApp();
	}
}

void Application::HandleInput()
{
	// Check if user wants to toggle UI visibility
	if (InputHandler::Instance()->GetKeyPressedOnce(Keyboard::Q))
	{
		m_UI->ToggleUI();
	}
}

void Application::UpdateApp()
{
	m_loadedScene->UpdateScene();

	if (m_UI->GetUiVisible())
	{
		// Check if loaded scene needs to change
		if (m_UI->GetSceneNum() != 0)
			SetScene(m_UI->GetSceneNum());
	}
}

void Application::RenderApp()
{
	TheOpenGLRenderer::Instance()->StartOfFrame();

	if (m_loadedScene)
		m_loadedScene->DrawScene();

	if (m_UI->GetUiVisible())
		m_UI->RenderUI();
	
	TheOpenGLRenderer::Instance()->EndOfFrame();
}

/// <summary>
/// Function changes the scene to specified scene number, unloads the currently loaded scene and refreshes the UI light buttons
/// </summary>
/// <param name="newSceneNumber">Specified scene to change to (use sceneNames enum)</param>
bool Application::SetScene(int newSceneNumber)
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
		if (newSceneFilePath == m_loadedScene->GetSceneName())
		{
			// Scene is already loaded
			return false;
		}
	}

	m_loadedScene = nullptr;
	m_UI->UpdateSceneHandle(nullptr);
	m_loadedScene = std::make_shared<Scene>(newSceneFilePath);

	if (m_loadedScene->LoadScene())
	{
		// Scene successfully loaded
		if (m_UI)
			m_UI->UpdateSceneHandle(m_loadedScene);

		return true;
	}

	// Scene failed to load
	return false;
}
