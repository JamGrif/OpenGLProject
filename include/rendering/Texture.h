#pragma once

#include "rendering/BaseAsset.h"

// Stores texture information and provides a way to load a texture by interfacing with TextureManager
class Texture :
	public BaseAsset
{
public:
	Texture();
	~Texture();

	void						readTextureFromFile();
	inline bool					loadTexture(); 

	void						Bind(unsigned int slot = 0) const;
	void						Unbind(unsigned int slot = 0) const;

	inline void					setFilePath(const std::string& filePath);

	inline const std::string&	getFilePath() const;

private:

	GLuint						m_texture;

	std::string					m_filePath;

	int							m_width, m_height, m_BPP;

	unsigned char* localbuffer;
};

// Stores currently loaded textures ensuring only one of each texture gets loaded. Provides a way to load a texture from a file
class TextureManager
{
public:

	static std::shared_ptr<Texture> retrieveTextureObject(const std::string& filePath);

	static void		readTexturesFromFile();
	static void		createTextures();

	static void		clearTextures();

private:

	static std::vector<std::shared_ptr<Texture>> m_loadedTextures;

	TextureManager(); // Private so a class of this can't be initialized
};
