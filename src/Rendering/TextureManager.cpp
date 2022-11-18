#include "pch.h"
#include "Rendering/TextureManager.h"

bool TextureManager::addTexture(const std::string& textureID, TextureType textureType)
{
	// Check if texture with ID already exists
	if (m_textureMap.find(textureID) != m_textureMap.end())
		return false;

	// Set texture filepath using textureID
	std::string textureFilepath = "res/textures/" + textureID + ".png";

	std::unique_ptr<Texture> pTexture = std::make_unique<Texture>();

	if (pTexture->parseTexture(textureFilepath, textureType))
	{
		m_textureMap.insert({ textureID, std::move(pTexture) });
		return true;
	}
	else
	{
		return false;
	}
}

bool TextureManager::addCubemap(const std::string& cubemapID)
{
	// Check if texture with ID already exists
	if (m_cubemapMap.find(cubemapID) != m_cubemapMap.end())
		return false;

	// Set each cubeface filepath using cubemapID
	CubemapFaces facesFilepath =
	{
		"res/textures/sky/" + cubemapID + "_right.png",
		"res/textures/sky/" + cubemapID + "_left.png",
		"res/textures/sky/" + cubemapID + "_top.png",
		"res/textures/sky/" + cubemapID + "_bottom.png",
		"res/textures/sky/" + cubemapID + "_front.png",
		"res/textures/sky/" + cubemapID + "_back.png",
	};

	m_cubemapMap.insert({ cubemapID, new Cubemap(facesFilepath) });

	return true;
}

void TextureManager::createAllTextures()
{
	for (const auto& [key, value] : m_textureMap)
	{
		value->createTexture();
	}
}

void TextureManager::bindTextureAtID(const std::string& textureID)
{
	if (m_textureMap.count(textureID))
	{
		m_textureMap.at(textureID)->bindTexture();
	}
}

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
