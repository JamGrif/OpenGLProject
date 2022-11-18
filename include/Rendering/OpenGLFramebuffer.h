#pragma once

enum class ScreenFilter
{
	START_OF_FILTER_ENUM    = 0,
	Default			= 1,
	Inverse			= 2,
	Greyscale		= 3,
	EdgeDetection	= 4,
	Weird			= 5,
	END_OF_FILTER_ENUM
};

//Interface for a framebuffer, allowing it to be written to or read from
class OpenGLFramebuffer
{
public:
	OpenGLFramebuffer(bool multisampled);
	~OpenGLFramebuffer();

	void			draw();

	void			bindFramebuffer();
	void			unbindFramebuffer();

	void			bindReadFramebuffer();
	void			bindWriteFramebuffer();

	void			copyToFramebuffer();

	void			setFrameFilter(ScreenFilter index);

	unsigned int	getFBO() const;

private:
	float m_quadVertices[24] =
	{
		//Pos		//Texcoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int	m_FBO;
	unsigned int	m_frameColourTexture;
	unsigned int	m_RBO;
	
	unsigned int	m_quadVBO;

	ScreenFilter				m_screenFilter;

	unsigned int	m_screenWidth, m_screenHeight;

	std::string m_shaderID;
};

