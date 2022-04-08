#include "pch.h"
#include "Rendering/Renderer.h"

#include "Rendering/OpenGLWindow.h"
#include "Core/EngineStatics.h"

#include <glm/ext/matrix_clip_space.hpp>

Renderer::Renderer()
	:m_status(false), m_appWindow(nullptr), m_projMatrix{ 1.0f }, m_appVAO(0)
{
	// GLFW
	if (!glfwInit())
	{
		PRINT_ERROR("GLFW failed to initialize");
		m_status = false;
		return;
	}

	// OpenGL Window
	m_appWindow = std::make_shared<OpenGLWindow>(1920, 1080, "OpenGL - Jamie", false);
	if (!m_appWindow->getWindowStatus())
	{
		PRINT_ERROR("OpenGL window failed to initialize");
		m_status = false;
		return;
	}
	EngineStatics::setAppWindow(m_appWindow);

	// GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		PRINT_ERROR("GLEW failed to initialize");
		m_status = false;
		return;
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

	m_status = true;
}

Renderer::~Renderer()
{
	m_appWindow = nullptr;
	EngineStatics::setAppWindow(nullptr);

	EngineStatics::setProjectionMatrix(nullptr);

	if (m_appVAO)
		glDeleteVertexArrays(1, &m_appVAO);

	glfwTerminate();
}

void Renderer::startOfFrame() const
{
	glClear(GL_DEPTH_BUFFER_BIT); // Clear the screen buffers
	glfwPollEvents();
}

void Renderer::swapBuffers() const
{
	glfwSwapBuffers(m_appWindow->getRaw());
}

void Renderer::draw(size_t indicesCount) const
{
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_INT, 0);
}

void Renderer::drawCubemap(size_t vertexCount) const
{
	// Disables writing to the depth buffer
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

void Renderer::drawTerrain(size_t vertexCount) const
{
	glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLsizei>(vertexCount));
	glDrawArrays(GL_PATCHES, 0, static_cast<GLsizei>(vertexCount));
}

bool Renderer::getStatus()
{
	return m_status;
}
