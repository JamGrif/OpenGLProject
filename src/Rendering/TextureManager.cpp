#include "pch.h"
#include "Rendering/TextureManager.h"

static const std::string TEXTURE_FILEPATH_PREFIX		= "res/textures/";
static const std::string TEXTURE_FILEPATH_SUFFIX		= ".png";

/// <summary>
/// Parse the specified texture and add it to the textureMap
/// IMPORTANT: Name of texture file must be the same as its textureID
/// IMPORTANT: When all textures have been parsed, CreateAllTextures() must be called before textures can be used
/// </summary>
bool TextureManager::AddTexture(const std::string& textureID, TextureType textureType)
{
	// Check if texture with ID already exists
	if (m_texturePool.find(textureID) != m_texturePool.end())
		return false;

	// Automatically set the filepath of texture
	std::string textureFilepath = TEXTURE_FILEPATH_PREFIX + textureID + TEXTURE_FILEPATH_SUFFIX;

	// Create and parse the texture file
	std::unique_ptr<Texture> pTexture = std::make_unique<Texture>();
	if (pTexture->ParseTexture(textureFilepath, textureType))
	{
		m_texturePool.insert({ textureID, std::move(pTexture) });
		return true;
	}
	else
	{
		PRINT_ERROR("Failed to parse texture at: {0}", textureFilepath);
		return true;
	}
}

/// <summary>
/// Create all textures that have been successfully parsed
/// </summary>
void TextureManager::CreateAllTextures()
{
	for (const auto& [textureID, texture] : m_texturePool)
	{
		// If texture isn't already created, create it
		if (!texture->GetIsCreated())
			texture->CreateTexture();
	}
}

/// <summary>
/// Binds the specified texture to the OpenGL state
/// </summary>
void TextureManager::BindTextureAtID(const std::string& textureID)
{
	if (m_texturePool.count(textureID))
	{
		m_texturePool.at(textureID)->BindTexture();
	}
}

/// <summary>
/// Unbinds the specified texture to the OpenGL state
/// </summary>
void TextureManager::UnbindTextureAtID(const std::string& textureID)
{
	if (m_texturePool.count(textureID))
	{
		m_texturePool.at(textureID)->UnbindTexture();
	}
}

/// <summary>
/// Deletes all textures
/// </summary>
void TextureManager::ClearAllTextures()
{
	m_texturePool.clear();
}
