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

//Interface for a framebuffer, allowing it to be written to or read from
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

	uint32_t		m_FBO;
	uint32_t		m_frameColourTexture;
	uint32_t		m_RBO;
	
	uint32_t		m_quadVBO;

	ScreenFilter	m_screenFilter;

	uint16_t		m_screenWidth, m_screenHeight;

	ResourceID		m_shaderID;
};

