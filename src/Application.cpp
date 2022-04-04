#include "pch.h"
#include "Application.h"

#include "Framebuffer.h"
#include "EngineStatics.h"
#include "OpenGLWindow.h"
#include "Scene.h"
#include "UI.h"


Application::Application()
	:m_appWindow(nullptr), m_projMatrix{ 1.0f }, m_appVAO(0),
	m_UI(nullptr), m_loadedScene(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr)
{
}

Application::~Application()
{
	EngineStatics::setProjectionMatrix(nullptr);

	if (m_appVAO)
		glDeleteVertexArrays(1, &m_appVAO);

	m_appWindow = nullptr;

	glfwTerminate();
}

/// <summary>
/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
/// </summary>
/// <returns>Returns success or failure of initialization</returns>
bool Application::appInit()
{
	Log::init();

	// GLFW
	if (!glfwInit())
	{
		PRINT_ERROR("GLFW failed to initialize");
		return false;
	}

	// OpenGL Window
	m_appWindow = std::make_shared<OpenGLWindow>(1920, 1080, "OpenGL - Jamie", false);
	if (!m_appWindow->getWindowStatus())
	{
		PRINT_ERROR("OpenGL window failed to initialize");
		return false;
	}
	EngineStatics::setAppWindow(m_appWindow);

	// GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		PRINT_ERROR("GLEW failed to initialize");
		return false;
	}

	PRINT_TRACE("Vendor is {0}", glGetString(GL_VENDOR));
	PRINT_TRACE("Renderer is {0}", glGetString(GL_RENDERER));
	PRINT_TRACE("Version is {0}", glGetString(GL_VERSION));

	/*
		Set OpenGL Context Settings
	*/

	// Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Backface Culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Multisampling
	glEnable(GL_MULTISAMPLE);

	// Build applications projection matrix
	m_projMatrix = glm::perspective(glm::radians(75.0f), m_appWindow->getAspectRatio(), 0.01f, 1000.0f);
	EngineStatics::setProjectionMatrix(&m_projMatrix);

	// Build applications VAO
	glGenVertexArrays(1, &m_appVAO);
	glBindVertexArray(m_appVAO);

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
	m_sceneFilterFramebuffer = std::make_unique<Framebuffer>(false);
	m_sceneMSAAFrameBuffer = std::make_unique<Framebuffer>(true);

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
	while (!glfwWindowShouldClose(m_appWindow->getWindow()))
	{
		ApplicationClock::tick();

		glClear(GL_DEPTH_BUFFER_BIT); // Clear the screen buffers
		glfwPollEvents();

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
			glBlitFramebuffer(0, 0, m_appWindow->getWindowWidth(), m_appWindow->getWindowHeight(), 0, 0, m_appWindow->getWindowWidth(), m_appWindow->getWindowHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
			m_sceneMSAAFrameBuffer->unbindFramebuffer();

			// Draw screen filter to buffer
			m_sceneFilterFramebuffer->draw();
		}

		if (m_UI)
			m_UI->drawInFrame();

		glfwSwapBuffers(m_appWindow->getWindow());

	}
}

/// <summary>
/// Function is called every time the window is resized
/// </summary>
/// <param name="window">The resized window</param>
/// <param name="newWidth">New width of the window after resizing</param>
/// <param name="newHeight">New height of the window after resizing</param>
void Application::windowResizeCALLBACK(GLFWwindow* window, int newWidth, int newHeight)
{
	PRINT_TRACE("called windowResizeCALLBACK function");
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

	//m_loadedScene = new Scene(newSceneFilePath);
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
