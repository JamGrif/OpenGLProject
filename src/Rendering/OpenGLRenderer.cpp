#include "pch.h"
#include "Rendering/OpenGLRenderer.h"

#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLErrorCheck.h"

#include <GL/glew.h> // glew.h must always be the first openGL header to be included (before glfw3.h)
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>


OpenGLRenderer::OpenGLRenderer()
	:m_projMatrix{ 1.0f }, m_appVAO(0)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::init()
{
	// GLFW
	if (!glfwInit())
	{
		PRINT_ERROR("GLFW failed to initialize");
		return false;
	}

	TheOpenGLWindow::Instance()->init(1920, 1080, "OpenGL - Jamie", false);

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
	glCall(glEnable(GL_DEPTH_TEST));
	glCall(glDepthFunc(GL_LEQUAL));

	// Back face Culling
	glCall(glEnable(GL_CULL_FACE));
	glCall(glFrontFace(GL_CCW));
	glCall(glCullFace(GL_BACK));

	// Multi sampling
	glCall(glEnable(GL_MULTISAMPLE));

	// Build applications projection matrix
	m_projMatrix = glm::perspective(glm::radians(75.0f), TheOpenGLWindow::Instance()->getAspectRatio(), 0.01f, 1000.0f);

	// Build applications VAO
	glCall(glGenVertexArrays(1, &m_appVAO));
	glCall(glBindVertexArray(m_appVAO));

	return true;
}

bool OpenGLRenderer::clean()
{
	if (m_appVAO)
		glDeleteVertexArrays(1, &m_appVAO);

	glCall(glfwTerminate());

	return true;
}

void OpenGLRenderer::startOfFrame() const
{
	glCall(glClear(GL_DEPTH_BUFFER_BIT)); // Clear the screen buffers
	glCall(glfwPollEvents());
}

void OpenGLRenderer::swapBuffers() const
{
	glCall(glfwSwapBuffers(TheOpenGLWindow::Instance()->getWindowPtr()));
}

/// <summary>
/// Assumes all appropriate meshes, shaders and textures have been binded correctly
/// </summary>
/// <param name="indicesCount"></param>
void OpenGLRenderer::draw(size_t indicesCount) const
{
	glCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_INT, 0));
}

void OpenGLRenderer::drawCubemap(size_t vertexCount) const
{
	// Disables writing to the depth buffer
	glCall(glDepthFunc(GL_LEQUAL));
	glCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	glCall(glDepthFunc(GL_LESS));
}

void OpenGLRenderer::drawTerrain(size_t vertexCount) const
{
	glCall(glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLsizei>(vertexCount)));
	glCall(glDrawArrays(GL_PATCHES, 0, static_cast<GLsizei>(vertexCount)));
}
