#pragma once

// Everything that uses TextureManager will use Texture and Cubemap
#include "Rendering/Texture.h"
#include "Rendering/Cubemap.h"

class TextureManager
{
public:

	bool addTexture(const std::string& textureID, TextureType textureType);
	bool addCubemap(const std::string& cubemapID);

	void createAllTextures();

	void	bindTextureAtID(const std::string& textureID);
	void	unbindTextureAtID(const std::string& textureID);

	Cubemap* getCubemapAtID(const std::string cubemapID);

	void clearAllTextures();
	void clearAllCubemaps();

	static TextureManager* Instance()
	{
		static TextureManager* s_pInstance = new TextureManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_textureMap;
	std::unordered_map<std::string, Cubemap*> m_cubemapMap;

	TextureManager() {}
	~TextureManager() {}

};
typedef TextureManager TheTextureManager;
