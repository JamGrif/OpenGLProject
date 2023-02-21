#include "pch.h"
#include "Rendering/Resource/Cubemap.h"

#include "GL/glew.h"
#include "stb_image/stb_image.h"

static constexpr int NO_CUBEMAP = 0;

static const std::string CUBEMAP_FILEPATH_PREFIX		= "res/textures/sky/";
static const std::string CUBEMAP_FILEPATH_SUFFIX_RIGHT	= "_right.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_LEFT	= "_left.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_TOP	= "_top.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_BOTTOM = "_bottom.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_FRONT	= "_front.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_BACK	= "_back.png";

Cubemap::Cubemap()
	:IResource(), m_textureSlot(0), m_width{ 0,0,0,0,0,0 }, m_height{ 0,0,0,0,0,0 }, m_BPP{ 0,0,0,0,0,0 },
	m_localbuffer{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }
{
}

Cubemap::~Cubemap()
{
	// Unbind and delete buffers
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, NO_CUBEMAP));
	glCall(glDeleteTextures(1, &m_OpenGLResourceID));
}

/// <summary>
/// 1 / 2 of cubemap creation
/// Parse the .png file of each cubemap face at filepath
/// </summary>
void Cubemap::Parse(const std::string& facesFilepath)
{
	// Automatically set the filepath of each cubemap face
	CubemapFaces facesFilepathArray =
	{
		CUBEMAP_FILEPATH_PREFIX + facesFilepath + CUBEMAP_FILEPATH_SUFFIX_RIGHT,
		CUBEMAP_FILEPATH_PREFIX + facesFilepath + CUBEMAP_FILEPATH_SUFFIX_LEFT,
		CUBEMAP_FILEPATH_PREFIX + facesFilepath + CUBEMAP_FILEPATH_SUFFIX_TOP,
		CUBEMAP_FILEPATH_PREFIX + facesFilepath + CUBEMAP_FILEPATH_SUFFIX_BOTTOM,
		CUBEMAP_FILEPATH_PREFIX + facesFilepath + CUBEMAP_FILEPATH_SUFFIX_FRONT,
		CUBEMAP_FILEPATH_PREFIX + facesFilepath + CUBEMAP_FILEPATH_SUFFIX_BACK,
	};


	for (unsigned int i = e_START_OF_CUBEFACE_ENUM; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		// Flips texture on Y-Axis
		stbi_set_flip_vertically_on_load_thread(0);

		m_localbuffer[i] = stbi_load(facesFilepathArray[i].c_str(), &m_width[i], &m_height[i], &m_BPP[i], 0);

		// Check if file loaded successfully
		if (stbi_failure_reason() == "can't fopen")
		{
			// Free any successfully created faces
			for (unsigned int j = e_START_OF_CUBEFACE_ENUM; j < e_END_OF_CUBEFACE_ENUM; j++)
			{
				stbi_image_free(m_localbuffer[i]);
			}

			PRINT_WARN("CUBEMAP-> {0} failed to parse cubemap face", facesFilepathArray[i]);
			return;
		}
	}
}

/// <summary>
/// 2 / 2 of cubemap creation
/// Use parsed cubemap face data to create OpenGL cubemap buffer
/// </summary>
void Cubemap::Create()
{
	// Generate texture buffer
	glCall(glGenTextures(1, &m_OpenGLResourceID));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_OpenGLResourceID));

	// Parse each cubemap face
	for (unsigned int i = e_START_OF_CUBEFACE_ENUM; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		glCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width[i], m_height[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localbuffer[i]));
		stbi_image_free(m_localbuffer[i]);
	}

	// What happens if texture is rendered on a different sized surface
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// What happens to texCoords outside 0-1 range
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	// Unbind
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, NO_CUBEMAP));

	m_bIsCreated = true;
}

/// <summary>
/// Binds the cubemap texture to the OpenGL context
/// </summary>
void Cubemap::Bind()
{
	glCall(glActiveTexture(GL_TEXTURE0 + m_textureSlot));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_OpenGLResourceID));
}

/// <summary>
/// Unbinds the cubemap texture from the OpenGL context
/// </summary>
void Cubemap::Unbind()
{
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, NO_CUBEMAP));
}

/// <summary>
/// What the cubemap will do when its reset
/// Unused in this class
/// </summary>
void Cubemap::Reset()
{
}
