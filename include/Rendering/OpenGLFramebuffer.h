#pragma once

enum class ScreenFilter
{
	START_OF_FILTER_ENUM    = 0,
	Default					= 1,
	Inverse					= 2,
	Greyscale				= 3,
	EdgeDetection			= 4,
	Weird					= 5,
	END_OF_FILTER_ENUM
};

/// <summary>
/// Interface for an OpenGL framebuffer buffer
/// Provides utility functions for reading and writing to a framebuffer
/// </summary>
class OpenGLFramebuffer
{
public:
	OpenGLFramebuffer(bool multisampled);
	~OpenGLFramebuffer();

	void			Draw();

	void			BindFramebuffer();
	void			UnbindFramebuffer();

	void			BindReadFramebuffer();
	void			BindWriteFramebuffer();

	void			CopyToFramebuffer();

	void			SetFrameFilter(ScreenFilter index);

private:
	float m_quadVertices[24] =
	{
		 // Pos		  // Texcoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	OpenGLIndex		m_FBO;
	uint32_t		m_frameColourTexture;
	OpenGLIndex		m_RBO;
	
	OpenGLIndex		m_quadVBO;

	ScreenFilter	m_screenFilter;

	uint16_t		m_screenWidth, m_screenHeight;

	ResourceID		m_shaderID;
};
