#include "Application.h"

#include <iostream>
#include <string>

#include "stb_image.h"
#include "EngineStatics.h"
#include "GameTimer.h"

Application::Application()
	:m_appWindow(nullptr), m_projMatrix{ 1.0f }, m_appVAO(0), m_loadedScene(nullptr), m_input(nullptr)
{
}

Application::~Application()
{
	EngineStatics::setProjectionMatrix(nullptr);

	delete m_loadedScene;
	m_loadedScene = nullptr;

	delete m_input;
	m_input = nullptr;

	if (m_appVAO != 0)
	{
		glDeleteVertexArrays(1, &m_appVAO);
	}
	

	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();

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
	//std::cout << "Application Initialized" << std::endl;

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


	/*
		IMGUI
	*/

	//IMGUI_CHECKVERSION(); // Check the version
	//
	//ImGui::CreateContext();	// Creating the imgui context
	//ImGuiIO& io = ImGui::GetIO();
	//(void)io;
	//
	//ImGui_ImplGlfw_InitForOpenGL(m_appWindow, true); // Connect imgui to glfw window
	//ImGui_ImplOpenGL3_Init("#version 430");
	//
	//ImGui::StyleColorsDark(); 

	// Build applications projection matrix
	constexpr float SixtyDegrees = 1.0472f; //1.0472 = 60 degrees
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
	// ----------------
	
	// Create scene object
	if (!changeScene("res/scenes/FMPscene.txt")) // Ensure scene loaded successfully
	{
		return false;
	}

	return true;
}

/// <summary>
/// Entire application loop of program
/// </summary>
void Application::appLoop()
{
	GameTimer gt;
	gt.startGameTimer();

	while (!glfwWindowShouldClose(m_appWindow->getWindow()))
	{
		// imgui
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		gt.updateGameTimer();

		glClear(GL_DEPTH_BUFFER_BIT); //Clears the screen buffers
		glfwPollEvents();

		checkForSceneChange();

		m_loadedScene->updateScene();

		//ImGui::Begin("Hello from begin");
		//ImGui::Text("Hello from text");
		//ImGui::End();

		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_appWindow->getWindow());

	}

	gt.stopGameTimer();
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


void Application::checkForSceneChange()
{
	if (Input::getKeyPressedOnce(GLFW_KEY_F1))
	{
		changeScene("res/scenes/FMPscene.txt");
	}
	if (Input::getKeyPressedOnce(GLFW_KEY_F2))
	{
		changeScene("res/scenes/jamieTest.txt");
	}
	if (Input::getKeyPressedOnce(GLFW_KEY_F3))
	{
		changeScene("res/scenes/materialTest.txt");
	}
	if (Input::getKeyPressedOnce(GLFW_KEY_F4))
	{
		changeScene("res/scenes/lightTest.txt");
	}
}

bool Application::changeScene(const std::string newSceneName)
{
	if (m_loadedScene != nullptr)
	{
		if (newSceneName == m_loadedScene->getSceneName())
		{
			// Scene is already loaded
			return false;
		}
		// A scene is already loaded, so delete it
		delete m_loadedScene;
		m_loadedScene = nullptr;
	}

	m_loadedScene = new Scene(newSceneName);

	if (m_loadedScene->loadScene())
	{
		// Scene successfully loaded
		return true;
	}

	// Scene failed to load
	return false;
}

