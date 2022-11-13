#pragma once

// Everything that uses TextureManager will use Texture and Cubemap
#include "Rendering/Texture.h"
#include "Rendering/Cubemap.h"

class TextureManager
{
public:

	bool createTexture(const std::string& textureID, const std::string& textureFilepath, TextureType textureType);
	bool createCubemap(const std::string& cubemapID, const std::string& cubemapFilepath);

	Texture* getTextureAtID(const std::string textureID);
	Cubemap* getCubemapAtID(const std::string cubemapID);

	void clearAllTextures();
	void clearAllCubemaps();

	static TextureManager* Instance()
	{
		static TextureManager* s_pInstance = new TextureManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, Texture*> m_textureMap;
	std::unordered_map<std::string, Cubemap*> m_cubemapMap;

	TextureManager() {}
	~TextureManager() {}

};
typedef TextureManager TheTextureManager;
