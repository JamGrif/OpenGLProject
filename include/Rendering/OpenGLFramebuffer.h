#pragma once

// Forward Declarations
class OpenGLShader;

enum screenFilters
{
	screen_Default			= 1,
	screen_Inverse			= 2,
	screen_Greyscale		= 3,
	screen_EdgeDetection	= 4,
	screen_Drugs			= 5,
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

	void			setFrameFilter(int index);

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

	int				m_screenFilter;

	unsigned int	m_screenWidth, m_screenHeight;

	std::shared_ptr<OpenGLShader> m_screenShader;
};

