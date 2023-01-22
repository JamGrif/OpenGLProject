#pragma once

// Everything that uses TextureManager will use Texture
#include "Rendering/Texture.h"

typedef std::unordered_map<std::string, std::unique_ptr<Texture>> TexturePool;

/// <summary>
/// Encapsulates and abstracts the creation, deletion and usage of Texture objects
/// Textures can only be used through this class
/// </summary>
class TextureManager
{
public:
	bool		AddTexture(const std::string& textureID, TextureType textureType);
	void		CreateAllTextures(); 

	void		BindTextureAtID(const std::string& textureID);
	void		UnbindTextureAtID(const std::string& textureID);

	void		ClearAllTextures();

	static TextureManager* Instance() // Get instance
	{
		static TextureManager* s_pInstance = new TextureManager;
		return s_pInstance;
	}
private:

	TexturePool m_texturePool;

	TextureManager() {}
	~TextureManager() {}
	TextureManager(const TextureManager&) = delete;
};
typedef TextureManager TheTextureManager;
