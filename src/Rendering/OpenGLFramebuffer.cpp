#include "pch.h"
#include "Rendering/OpenGLFramebuffer.h"

#include "Rendering/OpenGLWindow.h"
#include "Rendering/Resource/Manager/ResourceManager.h"

#include "GL/glew.h"

OpenGLFramebuffer::OpenGLFramebuffer(bool multisampled)
	:m_FBO(0), m_frameColourTexture(0), m_RBO(0), m_quadVBO(0), m_screenFilter(ScreenFilter::Default),
	m_screenWidth(TheOpenGLWindow::Get()->GetWindowWidth()), m_screenHeight(TheOpenGLWindow::Get()->GetWindowHeight())
{
	glCall(glGenFramebuffers(1, &m_FBO));
	glCall(glGenRenderbuffers(1, &m_RBO));

	// By binding to a GL_FRAMEBUFFER, all read and write framebuffer operations will be on newly bounded framebuffer
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
	glCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));

	glCall(glGenTextures(1, &m_frameColourTexture));

	// If specified create a framebuffer that can handle multiple samples in the same texel
	if (multisampled)
	{
		// Create multisampled frame texture
		glCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_frameColourTexture));
		glCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, m_screenWidth, m_screenHeight, GL_TRUE));
		glCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_frameColourTexture, 0));

		// Create render buffer object
		glCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight));
	}
	// Otherwise, create a normal framebuffer
	else
	{
		// Create frame texture
		glCall(glBindTexture(GL_TEXTURE_2D, m_frameColourTexture));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenWidth, m_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glCall(glBindTexture(GL_TEXTURE_2D, 0));
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameColourTexture, 0));

		// Create render buffer object
		glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screenWidth, m_screenHeight));
	}

	glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO));
	
	// Check status of newly created framebuffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		PRINT_WARN("FRAMEBUFFER-> Problem creating framebuffer");
		glCall(glDeleteFramebuffers(1, &m_FBO));
		glCall(glDeleteRenderbuffers(1, &m_RBO));
	}

	// Unbind all
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

	// Set framebuffer shader
	m_shaderID = "screenbufferShader";
	ShaderManager::Get()->AddResource(m_shaderID, "res/shaders/framebuffer-vertex.glsl", "res/shaders/framebuffer-fragment.glsl");

	// Create the VBO object the screen will be drawn to
	glCall(glGenBuffers(1, &m_quadVBO));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO));
	glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices), &m_quadVertices, GL_STATIC_DRAW));
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	glCall(glDeleteFramebuffers(1, &m_FBO));
	glCall(glDeleteRenderbuffers(1, &m_RBO));
}

/// <summary>
/// Draws the framebuffer content onto a quad which gets drawn on screen
/// </summary>
void OpenGLFramebuffer::Draw()
{
	ShaderManager::Get()->BindResourceAtID(m_shaderID);

	// Sets shader values, texture and vertex attributes
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("screenTexture", 0);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("screenFilter", static_cast<int>(m_screenFilter));

	glCall(glActiveTexture(GL_TEXTURE0));
	glCall(glBindTexture(GL_TEXTURE_2D, m_frameColourTexture));

	glCall(glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO));

	glCall(glEnableVertexAttribArray(0));
	glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	glCall(glEnableVertexAttribArray(1));
	glCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));

	glCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	ShaderManager::Get()->UnbindResourceAtID(m_shaderID);
}

/// <summary>
/// Binds framebuffer to pipeline, any draw calls will be applied to this buffer
/// </summary>
void OpenGLFramebuffer::BindFramebuffer()
{
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
	glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	glCall(glEnable(GL_DEPTH_TEST));
}

/// <summary>
/// Unbinds framebuffer and clears the colour buffer
/// </summary>
void OpenGLFramebuffer::UnbindFramebuffer()
{
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glCall(glClear(GL_COLOR_BUFFER_BIT));
	glCall(glDisable(GL_DEPTH_TEST));
}

/// <summary>
/// Binds the framebuffer to only read from draw calls
/// </summary>
void OpenGLFramebuffer::BindReadFramebuffer()
{
	glCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO));
}

/// <summary>
/// Binds the framebuffer to only write to framebuffer to draw calls
/// </summary>
void OpenGLFramebuffer::BindWriteFramebuffer()
{
	glCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO));
}

/// <summary>
/// Copy the contents of currently bound framebuffer to this framebuffer
/// </summary>
void OpenGLFramebuffer::CopyToFramebuffer()
{
	glCall(glBlitFramebuffer(0, 0, m_screenWidth, m_screenHeight, 0, 0, m_screenWidth, m_screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST));
}

/// <summary>
/// Specifies which filter will be used when presenting buffer to the screen (see enum in OpenGLFramebuffer.h)
/// </summary>
void OpenGLFramebuffer::SetFrameFilter(ScreenFilter index)
{
	if (index >= ScreenFilter::END_OF_FILTER_ENUM || index <= ScreenFilter::START_OF_FILTER_ENUM)
		return;
	
	m_screenFilter = index;
}
