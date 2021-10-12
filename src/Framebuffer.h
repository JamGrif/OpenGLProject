#pragma once

#include "Shader.h"

enum
{
	screen_Default = 1,
	screen_Inverse,
	screen_Greyscale,
	screen_EdgeDetection,
	screen_Drugs,
	END_OF_FILTER_ENUM
};

//Interface for a framebuffer, allowing it to be written to or read from
class Framebuffer
{
public:
	Framebuffer(bool multisampled);
	~Framebuffer();

	void			draw();

	void			bindFramebuffer();
	void			unbindFramebuffer();

	void			setFrameFilter(int index);

	unsigned int getFBO();

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

	Shader*			m_screenShader;

};

