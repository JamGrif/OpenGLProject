#pragma once

// Everything that uses TextureManager will use Texture and Cubemap
#include "Rendering/Texture.h"
#include "Rendering/Cubemap.h"

/// <summary>
/// Abstracts and handles the creation, deletion and usage of Texture and Cubemap objects
/// Textures and Cubemaps can only be used through this class
/// </summary>
class TextureManager
{
public:

	bool	addTexture(const std::string& textureID, TextureType textureType);
	bool	addCubemap(const std::string& cubemapID);

	void	createAllTextures();

	void	bindTextureAtID(const std::string& textureID);
	void	unbindTextureAtID(const std::string& textureID);

	Cubemap* getCubemapAtID(const std::string cubemapID);

	void	clearAllTextures();
	void	clearAllCubemaps();

	static TextureManager* Instance() // Get instance
	{
		static TextureManager* s_pInstance = new TextureManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textureMap;
	std::unordered_map<std::string, Cubemap*> m_cubemapMap;

	TextureManager() {}
	~TextureManager() {}
	TextureManager(const TextureManager&) = delete;
};
typedef TextureManager TheTextureManager;
