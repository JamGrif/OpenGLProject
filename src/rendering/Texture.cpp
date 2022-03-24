#include "rendering/Texture.h"

#include <iostream>

#include "stb_image.h"

std::vector<std::shared_ptr<Texture>> TextureManager::m_loadedTextures;

Texture::Texture()
	:m_texture(0), m_width(0), m_height(0), m_BPP(0), m_filePath("")
{
}

Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_texture);
}

void Texture::readTextureFromFile()
{
	stbi_set_flip_vertically_on_load_thread(1); // Flips texture on Y-Axis

	localbuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_BPP, 4);

	// Check if file loaded successfully
	if (stbi_failure_reason() == "can't fopen")
	{
		std::cout << "TEXTURE->" << m_filePath << " failed to load, using default texture - FAILURE" << std::endl;
		//return false;
	}
	else
	{
		std::cout << "TEXTURE->" << m_filePath << " successfully loaded - SUCCESS" << std::endl;
	}
}

bool Texture::loadTexture()
{
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localbuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localbuffer)
	{
		stbi_image_free(localbuffer);
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
void Texture::Unbind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/// <summary>
/// Loads a texture from the specified filepath and sets its parameters
/// </summary>
void Texture::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& Texture::getFilePath() const
{
	return m_filePath;
}

/*
	TextureManager 
*/

/// <summary>
/// Loads the specified texture, if texture already exists then it returns a pointer to it instead of reloading the same texture
/// </summary>
/// <param name="filePath"></param>
/// <returns>Pointer to the created texture</returns>
std::shared_ptr<Texture> TextureManager::retrieveTextureObject(const std::string& filePath)
{
	// Check if a texture object with the same filePath is already loaded
	for (const auto& t : m_loadedTextures)
	{
		if (t->getFilePath() == filePath)
		{
			return t;
		}
	}

	// Create new texture object
	auto newTexture = std::make_shared<Texture>();
	newTexture->setFilePath(filePath);
	m_loadedTextures.push_back(newTexture);

	return newTexture;
}


void TextureManager::readTexturesFromFile()
{
	for (auto& t : m_loadedTextures)
	{
		t->readTextureFromFile();
	}
}

void TextureManager::createTextures()
{
	for (auto& t : m_loadedTextures)
	{
		if (!t->loadTexture())
		{
			// If texture fails to load, then load the default missingtexture texture
			t->setFilePath("res/textures/missingtexture.png");
			t->loadTexture();
		}
	}
}

void TextureManager::clearTextures()
{
	m_loadedTextures.clear();
}
