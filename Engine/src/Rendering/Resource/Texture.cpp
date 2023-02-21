#include "pch.h"
#include "Rendering/Resource/Texture.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h" // Image loading
#include "GL/glew.h"

static constexpr int DESIRED_TEXTURE_CHANNELS = 4;
static constexpr int NO_TEXTURE = 0;

static const std::string TEXTURE_FILEPATH_PREFIX = "res/textures/";
static const std::string TEXTURE_FILEPATH_SUFFIX = ".png";

static const char* STBI_ERROR_MESSAGE = "can't fopen";

Texture::Texture()
	:IResource(), m_textureWidth(0), m_textureHeight(0), m_textureBPP(0),
	m_textureType(TextureType::UNSET), m_pTempBuffer(nullptr)
{
}

Texture::~Texture()
{
	// Unbind and delete buffers
	glCall(glBindTexture(GL_TEXTURE_2D, NO_TEXTURE));
	glCall(glDeleteTextures(1, &m_OpenGLResourceID));
}

/// <summary>
/// 1 / 2 of texture creation
/// Parse the .png file at filepath
/// </summary>
void Texture::Parse(const std::string& filepath)
{
	// Flips texture on Y-Axis
	stbi_set_flip_vertically_on_load_thread(true);

	// Automatically set the filepath of texture
	std::string textureFilepath = TEXTURE_FILEPATH_PREFIX + filepath + TEXTURE_FILEPATH_SUFFIX;

	m_pTempBuffer = stbi_load(textureFilepath.c_str(), &m_textureWidth, &m_textureHeight, &m_textureBPP, DESIRED_TEXTURE_CHANNELS);

	// Check result
	if (stbi_failure_reason() == STBI_ERROR_MESSAGE)
	{
		PRINT_WARN("TEXTURE-> {0} failed to parse texture", m_resourceFilepath);
		stbi_image_free(m_pTempBuffer);

		return;
	}

	m_resourceID = filepath;
	m_resourceFilepath = textureFilepath;
}

/// <summary>
/// 2 / 2 of texture creation
/// Use parsed texture data to create OpenGL texture buffers
/// </summary>
void Texture::Create()
{
	// Generate one texture buffer
	glCall(glGenTextures(1, &m_OpenGLResourceID));
	glCall(glBindTexture(GL_TEXTURE_2D, m_OpenGLResourceID));

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
	glCall(glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGBA, m_textureWidth, m_textureHeight, NULL, GL_RGBA, GL_UNSIGNED_BYTE, m_pTempBuffer));
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

/// <summary>
/// What the texture will do when its reset
/// Unused in this class
/// </summary>
void Texture::Reset()
{
}

/// <summary>
/// Bind the texture buffer to the OpenGL context
/// </summary>
void Texture::Bind()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, m_OpenGLResourceID));
}

/// <summary>
/// Unbind the texture buffer to the OpenGL context
/// </summary>
void Texture::Unbind()
{
	glCall(glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_textureType)));
	glCall(glBindTexture(GL_TEXTURE_2D, NO_TEXTURE));
}
