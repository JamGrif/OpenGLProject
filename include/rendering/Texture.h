#pragma once

#include "rendering/BaseAsset.h"

/// <summary>
/// Stores texture information and provides a way to load a texture by providing an interface for TextureManager
/// </summary>
class Texture :
	public BaseAsset
{
public:
	Texture();
	~Texture();

	// Drawing functions
	void						Bind(unsigned int slot = 0) const;
	void						Unbind(unsigned int slot = 0) const;

	// Texture loading functions
	inline bool					readTextureFromFile();
	inline void					loadTexture(); 

	inline void					setFilePath(const std::string& filePath);
	inline const std::string&	getFilePath() const;

private:

	GLuint						m_textureID;

	int							m_width, m_height, m_BPP;

	std::string					m_filePath;

	unsigned char*				m_localbuffer;
};

/// <summary>
/// Stores created texture objects, only one version of a texture object will exist for each filepath set.
/// Do all texture creation and deleting through the TextureManager
/// </summary>
class TextureManager
{
public:

	static std::shared_ptr<Texture> retrieveTextureObject(const std::string& filePath);

	static void						readTexturesFromFile();
	static void						createTextures();

	static void						clearTextures();

private:

	static std::vector<std::shared_ptr<Texture>> m_loadedTextures;

	TextureManager(); // Private so a class of this can't be initialized
};
