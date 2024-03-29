#include "pch.h"
#include "Core/Engine.h"

#include "Core/InputHandler.h"
#include "Core/UI.h"
#include "Scene/Scene.h"
#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLRenderer.h"

static constexpr int CONSOLE_STARTING_X = -900;
static constexpr int CONSOLE_STARTING_Y = 525;

static constexpr int CONSOLE_WIDTH = 900;
static constexpr int CONSOLE_HEIGHT = 500;

// Enable console if debug mode
#ifdef _DEBUG
	#include <windows.h>
	#define startConsole()																								\
		AllocConsole();																									\
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);																\
		MoveWindow(GetConsoleWindow(), CONSOLE_STARTING_X, CONSOLE_STARTING_Y, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);
#else
	#define startConsole()
#endif

namespace Engine
{
	static bool ENGINE_STARTED = false;
	static std::string sceneToLoad;

	bool StartEngine()
	{
		if (ENGINE_STARTED)
			return false;

		ENGINE_STARTED = true;

		startConsole();
		EngineMain* em = new EngineMain();

		if (em->Initialise(sceneToLoad))
			em->Loop();

		delete em;

		return false;
	}

	void SetInitalScene(const std::string& text)
	{
		sceneToLoad = text;
	}


	EngineMain::EngineMain()
		:m_UI(nullptr), m_loadedScene(nullptr)
	{
	}

	EngineMain::~EngineMain()
	{
		if (m_loadedScene)
			delete m_loadedScene;

		if (m_UI)
			delete m_UI;

		TheOpenGLWindow::Get()->Clean();

		TheOpenGLRenderer::Get()->Clean();
	}

	/// <summary>
	/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
	/// Called once at program start
	/// </summary>
	bool EngineMain::Initialise(const std::string& text)
	{
		// Initialize all systems used by application
		Log::Init();

		PRINT_WARN(text);

		TheOpenGLRenderer::Get()->Init();

		EngineClock::Get()->Init();

		InputHandler::Get()->Init();

		m_UI = new UI(false, &m_loadedScene);

		if (text.empty())
		{
			// Create Scene object and set initial scene
			if (!SetScene("materialTest"))
				return false;
		}
		else
		{
			// Create Scene object and set initial scene
			if (!SetScene(text))
				return false;
		}


		

		return true;
	}

	/// <summary>
	/// Entire loop of program
	/// Called every frame
	/// </summary>
	void EngineMain::Loop()
	{
		while (!TheOpenGLWindow::Get()->ShouldClose())
		{
			EngineClock::Get()->Tick();

			HandleInput();
			UpdateApp();
			RenderApp();
		}
	}

	/// <summary>
	/// User input functions of the application
	/// </summary>
	void EngineMain::HandleInput()
	{
		// Check if user wants to toggle UI visibility
		if (InputHandler::Get()->GetKeyPressedOnce(Keyboard::Q))
			m_UI->ToggleUI();
	}

	/// <summary>
	/// Update functions of the application
	/// </summary>
	void EngineMain::UpdateApp()
	{
		if (m_loadedScene)
			m_loadedScene->UpdateScene();
	}

	/// <summary>
	/// Render functions of the application
	/// </summary>
	void EngineMain::RenderApp()
	{
		TheOpenGLRenderer::Get()->StartOfFrame();

		if (m_loadedScene)
			m_loadedScene->RenderScene();

		if (m_UI)
			m_UI->RenderUI();

		TheOpenGLRenderer::Get()->EndOfFrame();
	}

	/// <summary>
	/// Changes the scene to specified scene name, unloads the currently loaded scene and refreshes the UI light buttons
	/// </summary>
	bool EngineMain::SetScene(const std::string& sceneName)
	{
		// Prevent reloading current scene
		if (m_loadedScene && sceneName == m_loadedScene->GetSceneName())
			return false;

		if (m_loadedScene)
			delete m_loadedScene;

		m_loadedScene = new Scene(sceneName);

		if (m_loadedScene->LoadScene())
		{
			if (m_UI)
				m_UI->ChangePanelScene();

			// Scene successfully loaded
			return true;
		}

		// Scene failed to load
		return false;
	}

}

