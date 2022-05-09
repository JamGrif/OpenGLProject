#include "pch.h"
#include "Rendering/Renderer.h"

#include "Core/EngineStatics.h"
#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLErrorCheck.h"

#include <GL/glew.h> // glew.h must always be the first openGL header to be included (before glfw3.h)
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>

// Scene.cpp
extern int entityDrawCount;

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
	glCall(glEnable(GL_DEPTH_TEST));
	glCall(glDepthFunc(GL_LEQUAL));

	// Backface Culling
	glCall(glEnable(GL_CULL_FACE));
	glCall(glFrontFace(GL_CCW));
	glCall(glCullFace(GL_BACK));

	// Multisampling
	glCall(glEnable(GL_MULTISAMPLE));

	// Build applications projection matrix
	m_projMatrix = glm::perspective(glm::radians(75.0f), m_appWindow->getAspectRatio(), 0.01f, 1000.0f);
	EngineStatics::setProjectionMatrix(&m_projMatrix);

	// Build applications VAO
	glCall(glGenVertexArrays(1, &m_appVAO));
	glCall(glBindVertexArray(m_appVAO));

	m_status = true;
}

Renderer::~Renderer()
{
	m_appWindow = nullptr;
	EngineStatics::setAppWindow(nullptr);

	EngineStatics::setProjectionMatrix(nullptr);

	if (m_appVAO)
		glDeleteVertexArrays(1, &m_appVAO);

	glCall(glfwTerminate());
}

void Renderer::startOfFrame() const
{
	glCall(glClear(GL_DEPTH_BUFFER_BIT)); // Clear the screen buffers
	glCall(glfwPollEvents());
}

void Renderer::swapBuffers() const
{
	glCall(glfwSwapBuffers(m_appWindow->getRaw()));
}

/// <summary>
/// Assumes all appropriate meshes, shaders and textures have been binded correctly
/// </summary>
/// <param name="indicesCount"></param>
void Renderer::draw(size_t indicesCount) const
{
	entityDrawCount++;

	glCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_INT, 0));
}

void Renderer::drawCubemap(size_t vertexCount) const
{
	// Disables writing to the depth buffer
	glCall(glDepthFunc(GL_LEQUAL));
	glCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	glCall(glDepthFunc(GL_LESS));
}

void Renderer::drawTerrain(size_t vertexCount) const
{
	glCall(glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLsizei>(vertexCount)));
	glCall(glDrawArrays(GL_PATCHES, 0, static_cast<GLsizei>(vertexCount)));
}

bool Renderer::getStatus()
{
	return m_status;
}
