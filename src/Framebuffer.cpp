#include "pch.h"
#include "Framebuffer.h"

#include "rendering/Shader.h"
#include "EngineStatics.h"
#include "OpenGLWindow.h"

Framebuffer::Framebuffer(bool multisampled)
	:m_FBO(0), m_frameColourTexture(0), m_RBO(0),
	m_quadVBO(0), m_screenFilter(1), m_screenShader(nullptr)
{
	glGenFramebuffers(1, &m_FBO);
	glGenRenderbuffers(1, &m_RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO); // By binding to a GL_FRAMEBUFFER, all read and write framebuffer operations will be on newly bounded framebuffer
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

	glGenTextures(1, &m_frameColourTexture);

	int windowWidth = EngineStatics::getAppWindow()->getWindowWidth();
	int windowHeight = EngineStatics::getAppWindow()->getWindowHeight();

	// If specified create a framebuffer that can handle multiple samples in the same texel
	if (multisampled)
	{
		// Create multisampled frame texture
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_frameColourTexture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, windowWidth, windowHeight, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_frameColourTexture, 0);

		// Create render buffer object
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	}
	// Otherwise, create a normal framebuffer
	else
	{
		// Create frame texture
		glBindTexture(GL_TEXTURE_2D, m_frameColourTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameColourTexture, 0);

		// Create render buffer object
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
	}
	
	// Check status of newly created framebuffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		PRINT_WARN("FRAMEBUFFER-> Problem creating framebuffer");
	}

	// Unbind all
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Set framebuffer shader
	m_screenShader = ShaderManager::retrieveShader("res/shaders/framebuffer-vertex.glsl", "res/shaders/framebuffer-fragment.glsl");

	// Create the VBO object the screen will be drawn to
	glGenBuffers(1, &m_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_quadVertices), &m_quadVertices, GL_STATIC_DRAW);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_FBO);
	//std::cout << "deleted framebuffer" << std::endl;
}

/// <summary>
/// Draws the framebuffer content onto a quad which gets drawn on screen
/// </summary>
void Framebuffer::draw()
{
	if (m_screenShader == nullptr) // No shader attached
	{
		return;
	}

	m_screenShader->Bind();

	// Sets shader values, texture and vertex attributes
	m_screenShader->setUniform1i("screenTexture", 0);
	m_screenShader->setUniform1i("screenFilter", m_screenFilter);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frameColourTexture);

	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_screenShader->Unbind();

}

/// <summary>
/// Binds framebuffer to pipeline, any draw calls will be applied to this buffer
/// </summary>
void Framebuffer::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

/// <summary>
/// Unbinds framebuffer and clears the colour buffer
/// </summary>
void Framebuffer::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
}

/// <summary>
/// Binds the framebuffer to only read from draw calls
/// </summary>
void Framebuffer::bindReadFramebuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
}

/// <summary>
/// Binds the framebuffer to only write to framebuffer to draw calls
/// </summary>
void Framebuffer::bindWriteFramebuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

/// <summary>
/// Specifies which filter will be used when presenting buffer to the screen (see enum in Framebuffer.h)
/// </summary>
/// <param name="index"></param>
void Framebuffer::setFrameFilter(int index)
{
	if (index >= END_OF_FILTER_ENUM || index <= 0)
	{
		return;
	}

	m_screenFilter = index;
}

/// <summary>
/// Returns the Framebuffer Object (FBO)
/// </summary>
/// <returns></returns>
unsigned int Framebuffer::getFBO() const
{
	return m_FBO;
}
