#include "pch.h"
#include "Rendering/TextureManager.h"

bool TextureManager::createTexture(const std::string& textureID, const std::string& textureFilepath, TextureType textureType)
{
	// Check if texture with ID already exists
	if (m_textureMap.find(textureID) != m_textureMap.end())
		return false;

	//PRINT_TRACE("created texture with ID {0}", textureID);

	Texture* pTexture = new Texture(textureFilepath, textureType);
	m_textureMap.insert({ textureID, pTexture });

	return true;
}

bool TextureManager::createCubemap(const std::string& cubemapID, const std::string& cubemapFilepath)
{
	// Check if texture with ID already exists
	if (m_cubemapMap.find(cubemapID) != m_cubemapMap.end())
		return false;

	//PRINT_TRACE("created cubemap with ID {0}", cubemapID);

	Cubemap* pCubemap = new Cubemap(cubemapFilepath);
	m_cubemapMap.insert({ cubemapID, pCubemap });

	return true;
}

Texture* TextureManager::getTextureAtID(const std::string textureID)
{
	return m_textureMap.find(textureID) != m_textureMap.end() ? m_textureMap.at(textureID) : nullptr;
}

Cubemap* TextureManager::getCubemapAtID(const std::string cubemapID)
{
	return m_cubemapMap.find(cubemapID) != m_cubemapMap.end() ? m_cubemapMap.at(cubemapID) : nullptr;
}

void TextureManager::clearAllTextures()
{
	for (const auto& [key, value] : m_textureMap)
	{
		delete value;
	}
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
