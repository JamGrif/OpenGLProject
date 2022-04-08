#include "pch.h"
#include "rendering/OpenGLTexture.h"

#include "stb_image/stb_image.h" // Image loading

#include <GL/glew.h>
//#include <GLFW/glfw3.h>


std::vector<std::shared_ptr<OpenGLTexture>> TextureManager::m_loadedTextures;

OpenGLTexture::OpenGLTexture()
	:m_textureID(0), m_textureSlot(-1), m_width(0), m_height(0), m_BPP(0), m_filePath(""), m_localbuffer(nullptr)
{
}

OpenGLTexture::~OpenGLTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_textureID);
}

/// <summary>
/// Binds the texture to the specified slot
/// </summary>
void OpenGLTexture::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_textureSlot);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

/// <summary>
/// Unbinds the texture from the specified slot
/// </summary>
void OpenGLTexture::Unbind() const
{
	glActiveTexture(GL_TEXTURE0 + m_textureSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void OpenGLTexture::setTextureSlot(int slot)
{
	m_textureSlot = slot;
}

/// <summary>
/// Uses the saved filepath (from .setFilePath()) to load the texture image into the object.
/// Must be called BEFORE .loadTexture()
/// </summary>
/// <returns>Returns false if image can't be found at filepath</returns>
bool OpenGLTexture::readTextureFromFile()
{
	stbi_set_flip_vertically_on_load_thread(1); // Flips texture on Y-Axis

	m_localbuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_BPP, 4);

	// Check if file loaded successfully
	if (stbi_failure_reason() == "can't fopen")
	{
		PRINT_WARN("TEXTURE-> {0} failed to load, using default texture", m_filePath);
		stbi_image_free(m_localbuffer);
		m_localbuffer = nullptr;

		return false;
	}

	return true;
}

/// <summary>
/// Turns the saved image data into an OpenGL texture.
/// Must be called AFTER .readTextureFromFile()
/// </summary>
void OpenGLTexture::loadTexture()
{
	// Generate texture buffer
	glGenTextures(1, &m_textureID);

	glBindTexture(GL_TEXTURE_2D, m_textureID);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localbuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_localbuffer)
	{
		stbi_image_free(m_localbuffer);
		m_localbuffer = nullptr;
	}
}

/// <summary>
/// Sets the textures filepath. Used by .readTextureFromFile()
/// </summary>
void OpenGLTexture::setFilePath(const std::string& filePath)
{
	m_filePath = filePath;
}

const std::string& OpenGLTexture::getFilePath() const
{
	return m_filePath;
}

/// <summary>
/// Creates a texture object, using the specified filepath.
/// If an object already exists with the same filepath, then return that instead of recreating the same object.
/// </summary>
/// <returns>Pointer to the created texture object</returns>
std::shared_ptr<OpenGLTexture> TextureManager::retrieveTextureObject(const std::string& filePath)
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
	auto newTexture = std::make_shared<OpenGLTexture>();
	newTexture->setFilePath(filePath);
	m_loadedTextures.push_back(newTexture);

	return newTexture;
}

/// <summary>
/// Calls the .readTextureFromFile() function for each texture object created from ::retrieveTextureObject(). This part reads the image data from the filepath and puts it into the object.
/// Part 1 of 2 for texture creation
/// </summary>
void TextureManager::readTexturesFromFile()
{
	for (auto& t : m_loadedTextures)
	{
		if (!t->readTextureFromFile())
		{
			// If texture fails to be read, then load in the default missingtexture texture
			t->setFilePath("res/textures/missingtexture.png");
			t->readTextureFromFile();
		}
	}
}

/// <summary>
/// Calls the .loadTexture() function for each texture object. Uses the saved image data saved in the object to create the OpenGL texture
/// Part 2 of 2 for texture creation
/// </summary>
void TextureManager::createTextures()
{
	for (auto& t : m_loadedTextures)
	{
		t->loadTexture();
	}
}

/// <summary>
/// Completely clears all saved texture objects
/// </summary>
void TextureManager::clearTextures()
{
	m_loadedTextures.clear();
}
