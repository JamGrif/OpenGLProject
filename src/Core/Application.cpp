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

	TheOpenGLWindow::Instance()->Clean();
}

/// <summary>
/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
/// Called once at program start
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
	m_UI = std::make_unique<UI>(false);

	// Create Scene object and set initial scene
	if (!SetScene(SceneName::jamieTest)) 
		return false;
	
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

/// <summary>
/// User input functions of the application
/// </summary>
void Application::HandleInput()
{
	// Check if user wants to toggle UI visibility
	if (InputHandler::Instance()->GetKeyPressedOnce(Keyboard::Q))
	{
		m_UI->ToggleUI();
	}
}

/// <summary>
/// Update functions of the application
/// </summary>
void Application::UpdateApp()
{
	m_loadedScene->UpdateScene();

	if (m_UI && m_UI->GetUiVisible())
	{
		// Check if loaded scene needs to change
		if (m_UI->GetCurrentSceneName() != SceneName::UNSET_SCENE)
			SetScene(m_UI->GetCurrentSceneName());
	}
}

/// <summary>
/// Render functions of the application
/// </summary>
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
bool Application::SetScene(SceneName newSceneNumber)
{
	std::string newSceneFilename = "";

	switch (static_cast<int>(newSceneNumber))
	{
		case static_cast<int>(SceneName::FMPscene):
			newSceneFilename = "FMPscene";
			break;
		case static_cast<int>(SceneName::jamieTest):
			newSceneFilename = "jamieTest";
			break;
		case static_cast<int>(SceneName::lightTest):
			newSceneFilename = "lightTest";
			break;
		case static_cast<int>(SceneName::materialTest):
			newSceneFilename = "materialTest";
			break;
		case static_cast<int>(SceneName::shadowTest):
			newSceneFilename = "shadowTest";
			break;
		default:
			PRINT_WARN("Specified sceneNumber is out of range:{0}", static_cast<int>(newSceneNumber));
			return false;
	}

	// Stop reloading current scene
	if (m_loadedScene)
		if (newSceneFilename == m_loadedScene->GetSceneName())
			return false;

	m_loadedScene = std::make_shared<Scene>(newSceneFilename);

	if (m_loadedScene->LoadScene())
	{
		if (m_UI)
			m_UI->UpdateSceneHandle(m_loadedScene);

		// Scene successfully loaded
		return true;
	}

	// Scene failed to load
	return false;
}
