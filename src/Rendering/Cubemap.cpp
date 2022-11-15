#include "pch.h"
#include "Rendering/Cubemap.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h"

#include <GL/glew.h>

Cubemap::Cubemap(const CubemapFaces& facesFilepath)
{
	//PRINT_TRACE("created cubemap at filepath {0}", m_filePath);

	for (unsigned int i = e_START_OF_CUBEFACE_ENUM; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		stbi_set_flip_vertically_on_load_thread(0);

		m_localbuffer[i] = stbi_load(facesFilepath[i].c_str(), &m_width[i], &m_height[i], &m_BPP[i], 0);

		// Check if file loaded successfully
		if (stbi_failure_reason() == "can't fopen")
		{
			PRINT_WARN("CUBEMAP-> {0} failed to load, loading default texture", facesFilepath[i]);
		}
	}

	// Generate texture buffer
	glCall(glGenTextures(1, &m_texture));

	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));

	for (unsigned int i = e_START_OF_CUBEFACE_ENUM; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		glCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width[i], m_height[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localbuffer[i]));
		stbi_image_free(m_localbuffer[i]);
	}


	// Specify what happens if texture is rendered on a different sized surface
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// Specify what happens to texCoords outside 0-1 range
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	// Unbind
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

Cubemap::~Cubemap()
{
	//PRINT_TRACE("deleted cubemap");
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	glCall(glDeleteTextures(1, &m_texture));
}

void Cubemap::bindCubemap()
{
	glCall(glActiveTexture(GL_TEXTURE0 + m_textureSlot));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));
}

void Cubemap::unbindCubemap()
{
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
