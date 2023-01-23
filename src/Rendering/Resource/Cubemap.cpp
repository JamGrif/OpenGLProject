#include "pch.h"
#include "Rendering/Resource/Cubemap.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h"

#include <GL/glew.h>

static constexpr int NO_CUBEMAP = 0;

Cubemap::Cubemap()
	:m_cubemapOpenGLID(0), m_textureSlot(0),
	m_width{ 0,0,0,0,0,0 }, m_height{ 0,0,0,0,0,0 }, m_BPP{ 0,0,0,0,0,0 },
	m_localbuffer{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }
{
}

Cubemap::~Cubemap()
{
	// Unbind and delete buffers
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, NO_CUBEMAP));
	glCall(glDeleteTextures(1, &m_cubemapOpenGLID));
}

/// <summary>
/// 1 / 2 of cubemap creation
/// Parse the .png file of each cubemap face at filepath
/// </summary>
bool Cubemap::ParseCubemap(const CubemapFaces& facesFilepath)
{
	for (unsigned int i = e_START_OF_CUBEFACE_ENUM; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		// Flips texture on Y-Axis
		stbi_set_flip_vertically_on_load_thread(0);

		m_localbuffer[i] = stbi_load(facesFilepath[i].c_str(), &m_width[i], &m_height[i], &m_BPP[i], 0);

		// Check if file loaded successfully
		if (stbi_failure_reason() == "can't fopen")
		{
			// Free any successfully created faces
			for (unsigned int j = e_START_OF_CUBEFACE_ENUM; j < e_END_OF_CUBEFACE_ENUM; j++)
			{
				stbi_image_free(m_localbuffer[i]);
			}

			PRINT_WARN("CUBEMAP-> {0} failed to parse cubemap face", facesFilepath[i]);
			return false;
		}
	}

	return true;
}

/// <summary>
/// 2 / 2 of cubemap creation
/// Use parsed cubemap face data to create OpenGL cubemap buffer
/// </summary>
void Cubemap::CreateCubemap()
{
	// Generate texture buffer
	glCall(glGenTextures(1, &m_cubemapOpenGLID));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapOpenGLID));

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
}

/// <summary>
/// Binds the cubemap texture to the OpenGL context
/// </summary>
void Cubemap::BindCubemap()
{
	glCall(glActiveTexture(GL_TEXTURE0 + m_textureSlot));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapOpenGLID));
}

/// <summary>
/// Unbinds the cubemap texture from the OpenGL context
/// </summary>
void Cubemap::UnbindCubemap()
{
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, NO_CUBEMAP));
}
