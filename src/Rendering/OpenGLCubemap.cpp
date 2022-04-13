#include "pch.h"
#include "rendering/OpenGLCubeMap.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "stb_image/stb_image.h"

#include <GL/glew.h>
//#include <GLFW/glfw3.h>


std::vector<std::shared_ptr<OpenGLCubemap>> CubemapManager::m_loadedCubemaps;

OpenGLCubemap::OpenGLCubemap()
	:m_texture(0), m_textureSlot(-1), m_filePath(""), m_facesFilePath{ "","","","","","" },
	m_width{ 0,0,0,0,0,0 }, m_height{ 0,0,0,0,0,0 }, m_BPP{ 0,0,0,0,0,0 }
{
}

OpenGLCubemap::~OpenGLCubemap()
{
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	glCall(glDeleteTextures(1, &m_texture));
}


void OpenGLCubemap::readCubemapFromFile()
{
	m_facesFilePath[e_cubeFaceRight] = "res/textures/sky/" + m_filePath + "_right.png";
	m_facesFilePath[e_cubeFaceLeft] = "res/textures/sky/" + m_filePath + "_left.png";
	m_facesFilePath[e_cubeFaceTop] = "res/textures/sky/" + m_filePath + "_top.png";
	m_facesFilePath[e_cubeFaceBottom] = "res/textures/sky/" + m_filePath + "_bottom.png";
	m_facesFilePath[e_cubeFaceFront] = "res/textures/sky/" + m_filePath + "_front.png";
	m_facesFilePath[e_cubeFaceBack] = "res/textures/sky/" + m_filePath + "_back.png";

	for (unsigned int i = e_cubeFaceRight; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		stbi_set_flip_vertically_on_load_thread(0);

		m_localbuffer[i] = stbi_load(m_facesFilePath[i].c_str(), &m_width[i], &m_height[i], &m_BPP[i], 0);

		// Check if file loaded successfully
		if (stbi_failure_reason() == "can't fopen")
		{
			PRINT_WARN("CUBEMAP-> {0} failed to load, loading default texture", m_facesFilePath[i]);
		}
	}
}

bool OpenGLCubemap::loadCubemap()
{
	// Generate texture buffer
	glCall(glGenTextures(1, &m_texture));

	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));

	for (unsigned int i = e_cubeFaceRight; i < e_END_OF_CUBEFACE_ENUM; i++)
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

	return true;
}

/// <summary>
/// Binds the cubemap to the rendering pipeline and to specified cubemap texture slot
/// </summary>
/// <param name="slot">Cubemap texture slot to bind to</param>
void OpenGLCubemap::Bind() const
{
	glCall(glActiveTexture(GL_TEXTURE0 + m_textureSlot));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture));
}

/// <summary>
/// Unbinds cubemap texture from the pipeline
/// </summary>
void OpenGLCubemap::Unbind() const
{
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void OpenGLCubemap::setTextureSlot(int slot)
{
	m_textureSlot = slot;
}

/// <summary>
/// Loads a cubemap texture from the specified filepath and sets its parameters
/// </summary>
/// <returns></returns>
void OpenGLCubemap::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& OpenGLCubemap::getFilePath() const
{
	return m_filePath;
}

/*
	CubemapManager
*/

/// <summary>
/// Loads the specified cubemap texture, if cubemap already exists then it returns a pointer to it instead of reloading the same cubemap texture
/// </summary>
/// <returns>Pointer to the created texture</returns>
std::shared_ptr<OpenGLCubemap> CubemapManager::retrieveCubeMapObject(const std::string& filePath)
{
	// If filepath is default (""), then return the first cubemap (which will be the cubemap the sky uses)
	if (filePath == "")
	{
		if (m_loadedCubemaps.size() > 0)
		{
			return m_loadedCubemaps.at(0);
		}
		else
		{
			// If m_loadedCubemaps is empty, then no sky cubemap has been loaded so return nullptr
			return nullptr;
		}
	}

	// Check if cubemap is already loaded
	for (const auto& cm : m_loadedCubemaps)
	{
		if (cm->getFilePath() == filePath)
		{
			return cm;
		}
	}

	// Create new cubemap object
	auto newCubemap = std::make_shared<OpenGLCubemap>();
	newCubemap->setFilePath(filePath);
	m_loadedCubemaps.push_back(newCubemap);

	return newCubemap;
}


void CubemapManager::readCubemapsFromFile()
{
	for (auto& cm : m_loadedCubemaps)
	{
		cm->readCubemapFromFile();
	}
}

void CubemapManager::createCubemaps()
{
	for (auto& cm : m_loadedCubemaps)
	{
		cm->loadCubemap();
	}
}

void CubemapManager::clearCubemaps()
{
	m_loadedCubemaps.clear();
}
