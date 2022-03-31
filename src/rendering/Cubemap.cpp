#include "pch.h"

#include "rendering/CubeMap.h"

#include "stb_image.h"

std::vector<std::shared_ptr<Cubemap>> CubemapManager::m_loadedCubemaps;

Cubemap::Cubemap()
	:m_texture(0), m_filePath(""), m_facesFilePath{ "","","","","","" },
	m_width{ 0,0,0,0,0,0 }, m_height{ 0,0,0,0,0,0 }, m_BPP{ 0,0,0,0,0,0 }
{
}

Cubemap::~Cubemap()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_texture);
}


void Cubemap::readCubemapFromFile()
{
	m_facesFilePath[e_cubeFaceRight] = "res/textures/sky/" + m_filePath + "_right.png";
	m_facesFilePath[e_cubeFaceLeft] = "res/textures/sky/" + m_filePath + "_left.png";
	m_facesFilePath[e_cubeFaceTop] = "res/textures/sky/" + m_filePath + "_top.png";
	m_facesFilePath[e_cubeFaceBottom] = "res/textures/sky/" + m_filePath + "_bottom.png";
	m_facesFilePath[e_cubeFaceFront] = "res/textures/sky/" + m_filePath + "_front.png";
	m_facesFilePath[e_cubeFaceBack] = "res/textures/sky/" + m_filePath + "_back.png";

	

	//unsigned char* localBuffer;
	for (unsigned int i = e_cubeFaceRight; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		stbi_set_flip_vertically_on_load_thread(0);

		m_localbuffer[i] = stbi_load(m_facesFilePath[i].c_str(), &m_width[i], &m_height[i], &m_BPP[i], 0);

		// Check if file loaded successfully
		if (stbi_failure_reason() == "can't fopen")
		{
			std::cout << "CUBEMAP->" << m_facesFilePath[i] << " failed to load, loading default texture - FAILURE" << std::endl;
			//stbi_image_free(m_localbuffer[i]);
			//return false;
		}
		else
		{
			std::cout << "CUBEMAP->" << m_facesFilePath[i] << " successfully loaded - SUCCESS" << std::endl;
		}
	}
}

bool Cubemap::loadCubemap()
{
	

	// Generate texture buffer
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	for (unsigned int i = e_cubeFaceRight; i < e_END_OF_CUBEFACE_ENUM; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width[i], m_height[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localbuffer[i]);
		stbi_image_free(m_localbuffer[i]);
	}
	

	// Specify what happens if texture is rendered on a different sized surface
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Specify what happens to texCoords outside 0-1 range
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

/// <summary>
/// Binds the cubemap to the rendering pipeline and to specified cubemap texture slot
/// </summary>
/// <param name="slot">Cubemap texture slot to bind to</param>
void Cubemap::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

/// <summary>
/// Unbinds cubemap texture from the pipeline
/// </summary>
void Cubemap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

/// <summary>
/// Loads a cubemap texture from the specified filepath and sets its parameters
/// </summary>
/// <returns></returns>
void Cubemap::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& Cubemap::getFilePath() const
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
std::shared_ptr<Cubemap> CubemapManager::retrieveCubeMapObject(const std::string& filePath)
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
	auto newCubemap = std::make_shared<Cubemap>();
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
