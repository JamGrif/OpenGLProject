#include "Application.h"

#include <iostream>
#include <string>

#include "Framebuffer.h"
#include "EngineStatics.h"
#include "GameTimer.h"
#include "OpenGLWindow.h"
#include "Input.h"
#include "Scene.h"
#include "UI.h"

Application::Application()
	:m_appWindow(nullptr), m_projMatrix{ 1.0f }, m_appVAO(0),
	m_input(nullptr), m_UI(nullptr), m_loadedScene(nullptr), m_sceneMSAAFrameBuffer(nullptr), m_sceneFilterFramebuffer(nullptr), m_gameTimer(nullptr)
{
}

Application::~Application()
{
	EngineStatics::setProjectionMatrix(nullptr);

	delete m_gameTimer;
	m_gameTimer = nullptr;

	delete m_sceneFilterFramebuffer;
	m_sceneFilterFramebuffer = nullptr;

	delete m_sceneMSAAFrameBuffer;
	m_sceneMSAAFrameBuffer = nullptr;

	delete m_loadedScene;
	m_loadedScene = nullptr;

	delete m_UI;
	m_UI = nullptr;

	delete m_input;
	m_input = nullptr;

	if (m_appVAO != 0)
	{
		glDeleteVertexArrays(1, &m_appVAO);
	}

	delete m_appWindow;
	m_appWindow = nullptr;

	glfwTerminate();

	//std::cout << "Application Destroyed" << std::endl;
}

/// <summary>
/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
/// </summary>
/// <returns>Returns success or failure of initialization</returns>
bool Application::appInit()
{
	/*
		Initialize GLFW
	*/

	if (!glfwInit())
	{
		std::cout << "GLFW failed to initialize" << std::endl;
		return false;
	}

	/*
		Initialize Window
	*/

	m_appWindow = new OpenGLWindow(1920, 1080, "OpenGL - Jamie", false);
	//m_appWindow = new OpenGLWindow(1920, 1080, "OpenGL - Jamie", true);
	if (!m_appWindow->getWindowStatus())
	{
		std::cout << "Window failed to initialize" << std::endl;
		return false;
	}

	/*
		Initialize GLEW
	*/

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLFW failed to initialize" << std::endl;
		return false;
	}

	std::cout << "Vendor is " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer is " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version is " << glGetString(GL_VERSION) << std::endl << std::endl;

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
	//constexpr float SixtyDegrees = 1.0472f; //1.0472 = 60 degrees
	m_projMatrix = glm::perspective(glm::radians(75.0f), m_appWindow->getAspectRatio(), 0.01f, 1000.0f);
	EngineStatics::setProjectionMatrix(&m_projMatrix);

	// Build applications VAO
	glGenVertexArrays(1, &m_appVAO);
	glBindVertexArray(m_appVAO);

	/*
		Create applications objects
	*/

	// Create Input object
	m_input = new Input();

	// Create UI object
	m_UI = new UI(true);
	
	// Create Scene object
	if (!changeScene(e_shadowTest)) // load shadowTest.txt by default
	{
		// Scene failed to load
		return false;
	}

	// Create the apps Framebuffers
	m_sceneFilterFramebuffer = new Framebuffer(false);
	m_sceneMSAAFrameBuffer = new Framebuffer(true);

	// Create Game Timer object
	m_gameTimer = new GameTimer();

	return true;
}

/// <summary>
/// Entire application loop of program
/// </summary>
void Application::appLoop()
{
	if (m_gameTimer)
		m_gameTimer->startGameTimer();

	while (!glfwWindowShouldClose(m_appWindow->getWindow()))
	{
		if (m_gameTimer)
			m_gameTimer->updateGameTimer();

		glClear(GL_DEPTH_BUFFER_BIT); // Clear the screen buffers
		glfwPollEvents();

		if (Input::getKeyPressedOnce(GLFW_KEY_Q))
		{
			if (m_UI)
				m_UI->toggleUI();
		}

		if (m_UI)
		{
			if (m_UI->getSceneNum() != 0)
				changeScene(m_UI->getSceneNum());

			if (m_UI->getFilterNum() != 0)
				changeScreenFilter(m_UI->getFilterNum());

			m_UI->startOfFrame();
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

	m_gameTimer->stopGameTimer();
}

/// <summary>
/// Function is called every time the window is resized
/// </summary>
/// <param name="window">The resized window</param>
/// <param name="newWidth">New width of the window after resizing</param>
/// <param name="newHeight">New height of the window after resizing</param>
void Application::windowResizeCALLBACK(GLFWwindow* window, int newWidth, int newHeight)
{
	std::cout << "called windowResizeCALLBACK function" << std::endl;
}

bool Application::changeScene(int newSceneNumber)
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
		// Specified sceneNumber is out of range
		std::cout << "Specified sceneNumber is out of range" << std::endl;
		return false;
	}

	if (m_loadedScene != nullptr)
	{
		if (newSceneFilePath == m_loadedScene->getSceneName())
		{
			// Scene is already loaded
			return false;
		}
		// A scene is already loaded, so delete it
		delete m_loadedScene;
		m_loadedScene = nullptr;
	}

	m_loadedScene = new Scene(newSceneFilePath);

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

void Application::changeScreenFilter(int newFilterNumber)
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
	};
}

