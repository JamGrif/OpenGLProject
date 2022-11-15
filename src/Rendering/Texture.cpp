#include "pch.h"
#include "Rendering/Texture.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h" // Image loading

#include <GL/glew.h>

Texture::Texture()
{
	//PRINT_TRACE("created texture object with id of {0}", m_textureID);
}

Texture::~Texture()
{
	//PRINT_TRACE("deleted texture");
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	glCall(glDeleteTextures(1, &m_textureID));
}

void Texture::parseTexture(const std::string& filepath, TextureType textureType)
{
	m_filePath = filepath;
	stbi_set_flip_vertically_on_load_thread(1); // Flips texture on Y-Axis

	m_localbuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_BPP, 4);

	// Check if file loaded successfully
	if (stbi_failure_reason() == "can't fopen")
	{
		PRINT_WARN("TEXTURE-> {0} failed to load, using default texture", m_filePath);
		stbi_image_free(m_localbuffer);
		m_localbuffer = nullptr;

		return;
	}

	m_textureType = textureType;
}

void Texture::createTexture()
{
	// Generate texture buffer
	glCall(glGenTextures(1, &m_textureID));

	glCall(glBindTexture(GL_TEXTURE_2D, m_textureID));

	// Specify what happens if texture is rendered on a different sized surface
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// Specify what happens to texCoords outside 0-1 range
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

	// Anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //Ensure supported
	{
		GLfloat anisoSetting = 0.0f;
		glCall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting)); //Set anisoSetting to maximum sampling support
		glCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting)); //Apply Anisotropic Filtering
	}

	// Define the texture
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localbuffer));
	glCall(glGenerateMipmap(GL_TEXTURE_2D));

	// Unbind
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_localbuffer)
	{
		stbi_image_free(m_localbuffer);
		m_localbuffer = nullptr;
	}

	//PRINT_TRACE("created texture at filepath {0}", m_filePath);
}

void Texture::bindTexture()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture::unbindTexture()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}
