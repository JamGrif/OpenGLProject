#include "pch.h"
#include "Rendering/Texture.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h" // Image loading
#include "GL/glew.h"

static constexpr int DESIRED_TEXTURE_CHANNELS = 4;
static constexpr int NO_TEXTURE = 0;

Texture::Texture()
	:m_textureOpenGLID(-1), m_textureWidth(0),
	m_textureHeight(0), m_textureBPP(0),
	m_textureType(TextureType::UNSET), m_pTempBuffer(nullptr), m_bIsCreated(false)
{
}

Texture::~Texture()
{
	glCall(glBindTexture(GL_TEXTURE_2D, NO_TEXTURE));
	glCall(glDeleteTextures(1, &m_textureOpenGLID));
}

bool Texture::ParseTexture(const std::string& filepath, TextureType textureType)
{
	// Flips texture on Y-Axis
	stbi_set_flip_vertically_on_load_thread(true);

	m_pTempBuffer = stbi_load(filepath.c_str(), &m_textureWidth, &m_textureHeight, &m_textureBPP, DESIRED_TEXTURE_CHANNELS);

	// Check result
	if (stbi_failure_reason() == "can't fopen")
	{
		PRINT_WARN("TEXTURE-> {0} failed to parse texture", m_textureFilePath);
		stbi_image_free(m_pTempBuffer);

		return false;
	}

	m_textureFilePath = filepath;
	m_textureType = textureType;

	return true;
}

void Texture::CreateTexture()
{
	// Generate one texture buffer
	glCall(glGenTextures(1, &m_textureOpenGLID));
	glCall(glBindTexture(GL_TEXTURE_2D, m_textureOpenGLID));

	// What happens if texture is rendered on a different sized surface
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// What happens to texCoords outside 0-1 range
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

	// Anisotropic filtering if supported
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		GLfloat anisoSetting = 0.0f;

		//Set anisoSetting to maximum sampling support
		glCall(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting));

		//Apply Anisotropic Filtering
		glCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting)); 
	}

	// Define the texture
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureWidth, m_textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pTempBuffer));
	glCall(glGenerateMipmap(GL_TEXTURE_2D));

	// Unbind
	glCall(glBindTexture(GL_TEXTURE_2D, NO_TEXTURE));

	if (m_pTempBuffer)
	{
		stbi_image_free(m_pTempBuffer);
		m_pTempBuffer = nullptr;
	}

	m_bIsCreated = true;
}

void Texture::BindTexture()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, m_textureOpenGLID));
}

void Texture::UnbindTexture()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, NO_TEXTURE));
}
