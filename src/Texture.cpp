#include "Texture.h"

#include <iostream>

#include "stb_image.h"

std::vector<Texture*> TextureManager::m_loadedTextures;
std::vector<CubeMap*> TextureManager::m_loadedCubemaps;

Texture::Texture()
	:m_texture(0), m_width(0), m_height(0), m_BPP(0), m_filePath("")
{
}

Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_texture);
}

/// <summary>
/// Loads a texture from the specified filepath and sets its parameters
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
bool Texture::loadTexture(const std::string& filePath)
{
	m_filePath = filePath;

	unsigned char* localBuffer;

	stbi_set_flip_vertically_on_load(1); // Flips texture on Y-Axis
	localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_BPP, 4);

	// Check if file loaded successfully
	if (stbi_failure_reason() == "can't fopen")
	{
		std::cout << "TEXTURE->" << m_filePath << " failed to load, loading default texture - FAILURE" << std::endl;
		return false;
	}
	else
	{
		std::cout << "TEXTURE->" << m_filePath << " successfully loaded - SUCCESS" << std::endl;
	}

	// Generate texture buffer
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Specify what happens if texture is rendered on a different sized surface
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Specify what happens to texCoords outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// Anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) //Ensure supported
	{
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting); //Set anisoSetting to maximum sampling support
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting); //Apply Anisotropic Filtering
	}

	// Define the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer)
	{
		stbi_image_free(localBuffer);
	}

	return true;
}

/// <summary>
/// Binds the texture to the rendering pipeline and to specified texture slot
/// </summary>
/// <param name="slot">Texture slot to bind to</param>
void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

/// <summary>
/// Unbinds texture from the pipeline
/// </summary>
void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

/// <summary>
/// Unbinds texture from the pipeline
/// </summary>
void Texture::Unbind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

const std::string& Texture::getFilePath() const
{
	return m_filePath;
}

const GLuint Texture::getTexture() const
{
	return m_texture;
}

/// <summary>
/// Loads the specified texture, if texture already exists then it returns a pointer to it instead of reloading the same texture
/// </summary>
/// <param name="filePath"></param>
/// <returns>Pointer to the created texture</returns>
Texture* TextureManager::retrieveTexture(const std::string& filePath)
{
	// Check if texture is already loaded
	for (Texture* t : m_loadedTextures)
	{
		if (t->getFilePath() == filePath)
		{
			return t;
		}
	}

	//  Otherwise, create new texture
	Texture* newTexture = new Texture;

	if (!newTexture->loadTexture(filePath)) //Attempt to load texture
	{
		// Texture failed to load so check if missing texture texture is already loaded and then return it
		for (Texture* t : m_loadedTextures)
		{
			if (t->getFilePath() == "res/textures/missingtexture.png")
			{
				delete newTexture;
				newTexture = nullptr;

				return t;
			}
		}

		// The missing texture texture has not already been made so make it
		newTexture->loadTexture("res/textures/missingtexture.png");
	}

	m_loadedTextures.push_back(newTexture);
	return m_loadedTextures.back();
}

/// <summary>
/// Loads the specified cubemap texture, if texture already exists then it returns a pointer to it instead of reloading the same cubemap texture
/// </summary>
/// <returns>Pointer to the created texture</returns>
CubeMap* TextureManager::retrieveCubeMap(const std::string& filePath)
{
	// If a loaded cubemap exists, return a pointer to it
	if (m_loadedCubemaps.size() > 0)
	{
		return m_loadedCubemaps.back();
	}

	// Otherwise create a new cubemap
	CubeMap* t = new CubeMap;
	t->loadCubeMap(filePath);
	m_loadedCubemaps.push_back(t);
	return m_loadedCubemaps.back();
}

void TextureManager::clearTextures()
{
	for (Texture* t : m_loadedTextures)
	{
		delete t;
		t = nullptr;
	}
	m_loadedTextures.clear();
}

void TextureManager::clearCubemaps()
{
	for (CubeMap* cm : m_loadedCubemaps)
	{
		delete cm;
		cm = nullptr;
	}
	m_loadedCubemaps.clear();
}

CubeMap::CubeMap()
{
	m_skyFaces.reserve(6);
}

CubeMap::~CubeMap()
{
}

/// <summary>
/// Binds the cubemap to the rendering pipeline and to specified cubemap texture slot
/// </summary>
/// <param name="slot">Cubemap texture slot to bind to</param>
void CubeMap::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

/// <summary>
/// Unbinds cubemap texture from the pipeline
/// </summary>
void CubeMap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

/// <summary>
/// Loads a cubemap texture from the specified filepath and sets its parameters
/// </summary>
/// <returns></returns>
bool CubeMap::loadCubeMap(const std::string& filePath)
{
	m_skyFaces.push_back("res/textures/sky/" + filePath + "_right.png");
	m_skyFaces.push_back("res/textures/sky/" + filePath + "_left.png");
	m_skyFaces.push_back("res/textures/sky/" + filePath + "_top.png");
	m_skyFaces.push_back("res/textures/sky/" + filePath + "_bottom.png"); 
	m_skyFaces.push_back("res/textures/sky/" + filePath + "_front.png");
	m_skyFaces.push_back("res/textures/sky/" + filePath + "_back.png");

	unsigned char* localBuffer;

	stbi_set_flip_vertically_on_load(0); //Flips texture on Y-Axis

	//Generate texture buffer
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	for (unsigned int i = 0; i < m_skyFaces.size(); i++)
	{
		localBuffer = stbi_load(m_skyFaces[i].c_str(), &m_width, &m_height, &m_BPP, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
		stbi_image_free(localBuffer);
	}

	//Specify what happens if texture is rendered on a different sized surface
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Specify what happens to texCoords outside 0-1 range
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	//Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}
