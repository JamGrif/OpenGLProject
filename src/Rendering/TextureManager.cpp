#include "pch.h"
#include "Rendering/TextureManager.h"

static const std::string TEXTURE_FILEPATH_PREFIX		= "res/textures/";
static const std::string TEXTURE_FILEPATH_SUFFIX		= ".png";

static const std::string CUBEMAP_FILEPATH_PREFIX		= "res/textures/sky/";
static const std::string CUBEMAP_FILEPATH_SUFFIX_RIGHT	= "_right.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_LEFT	= "_left.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_TOP	= "_top.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_BOTTOM = "_bottom.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_FRONT	= "_front.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_BACK	= "_back.png";

/// <summary>
/// Parse the specified texture and add it to the textureMap
/// IMPORTANT: Name of texture file must be the same as its textureID
/// IMPORTANT: When all textures have been parsed, CreateAllTextures() must be called before textures can be used
/// </summary>
bool TextureManager::addTexture(const std::string& textureID, TextureType textureType)
{
	// Check if texture with ID already exists
	if (m_textureMap.find(textureID) != m_textureMap.end())
		return false;

	// Automatically set the filepath of texture
	std::string textureFilepath = TEXTURE_FILEPATH_PREFIX + textureID + TEXTURE_FILEPATH_SUFFIX;

	// Create and parse the texture file
	std::unique_ptr<Texture> pTexture = std::make_unique<Texture>();
	if (pTexture->parseTexture(textureFilepath, textureType))
	{
		m_textureMap.insert({ textureID, std::move(pTexture) });
		return true;
	}
	else
	{
		PRINT_ERROR("Failed to parse texture at: {0}", textureFilepath);
		return false;
	}
}

/// <summary>
/// Parse the specified cubemap and add it to the cubemapMap
/// IMPORTANT: Name of cubemap texture file must be the same as its cubemapID
/// </summary>
bool TextureManager::addCubemap(const std::string& cubemapID)
{
	// Check if texture with ID already exists
	if (m_cubemapMap.find(cubemapID) != m_cubemapMap.end())
		return false;

	// Automatically set the filepath of each cubemap face
	CubemapFaces facesFilepath =
	{
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_RIGHT,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_LEFT,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_TOP,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_BOTTOM,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_FRONT,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_BACK,
	};

	// Create cubemap and insert into map
	m_cubemapMap.insert({ cubemapID, new Cubemap(facesFilepath) });

	return true;
}

/// <summary>
/// Create all textures that have been successfully parsed
/// </summary>
void TextureManager::createAllTextures()
{
	for (const auto& [key, value] : m_textureMap)
	{
		value->createTexture();
	}
}

/// <summary>
/// Binds the specified texture to the OpenGL state
/// </summary>
void TextureManager::bindTextureAtID(const std::string& textureID)
{
	if (m_textureMap.count(textureID))
	{
		m_textureMap.at(textureID)->bindTexture();
	}
}

/// <summary>
/// Unbinds the specified texture to the OpenGL state
/// </summary>
void TextureManager::unbindTextureAtID(const std::string& textureID)
{
	if (m_textureMap.count(textureID))
	{
		m_textureMap.at(textureID)->unbindTexture();
	}
}

Cubemap* TextureManager::getCubemapAtID(const std::string cubemapID)
{
	return m_cubemapMap.find(cubemapID) != m_cubemapMap.end() ? m_cubemapMap.at(cubemapID) : nullptr;
}

/// <summary>
/// Deletes all textures in the textureMap
/// </summary>
void TextureManager::clearAllTextures()
{
	m_textureMap.clear();
}

void TextureManager::clearAllCubemaps()
{
	for (const auto& [key, value] : m_cubemapMap)
	{
		delete value;
	}
	m_cubemapMap.clear();
}
