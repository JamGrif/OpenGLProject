#include "pch.h"
#include "Rendering/Texture.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h" // Image loading

#include <GL/glew.h>

Texture::Texture()
	:m_textureOpenGLID(-1), m_textureWidth(0), m_textureHeight(0), m_textureBPP(0),
	m_textureType(TextureType::UNSET_TEXTURE), m_pLocalTempBuffer(nullptr)
{
}

Texture::~Texture()
{
	//PRINT_TRACE("deleting texture object with filepath of {0}", m_textureFilePath);

	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	glCall(glDeleteTextures(1, &m_textureOpenGLID));
}

bool Texture::parseTexture(const std::string& filepath, TextureType textureType)
{
	//PRINT_TRACE("parsing texture object with filepath of {0}", filepath);

	m_textureFilePath = filepath;

	stbi_set_flip_vertically_on_load_thread(1); // Flips texture on Y-Axis
	m_pLocalTempBuffer = stbi_load(m_textureFilePath.c_str(), &m_textureWidth, &m_textureHeight, &m_textureBPP, 4);

	// Check if file loaded successfully
	if (stbi_failure_reason() == "can't fopen")
	{
		PRINT_WARN("TEXTURE-> {0} failed to load, using default texture", m_textureFilePath);
		stbi_image_free(m_pLocalTempBuffer);
		m_pLocalTempBuffer = nullptr;

		return false;
	}

	m_textureType = textureType;

	return true;
}

void Texture::createTexture()
{
	// Generate texture buffer
	glCall(glGenTextures(1, &m_textureOpenGLID));

	glCall(glBindTexture(GL_TEXTURE_2D, m_textureOpenGLID));

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
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pLocalTempBuffer));
	glCall(glGenerateMipmap(GL_TEXTURE_2D));

	// Unbind
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_pLocalTempBuffer)
	{
		stbi_image_free(m_pLocalTempBuffer);
		m_pLocalTempBuffer = nullptr;
	}
}

void Texture::bindTexture()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, m_textureOpenGLID));
}

void Texture::unbindTexture()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}
