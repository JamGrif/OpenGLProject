#pragma once

enum class TextureType
{
	UNSET_TEXTURE		= -1,
	DIFFUSE_TEXTURE		= 0,
	SPECULAR_TEXTURE	= 1,
	EMISSION_TEXTURE	= 2,
	NORMAL_TEXTURE		= 3,
	HEIGHT_TEXTURE		= 4,
};


class Texture
{
public:
	Texture();
	~Texture();

	void parseTexture(const std::string& filepath, TextureType textureType);
	void createTexture();

	void bindTexture();
	void unbindTexture();

private:

	uint32_t					m_textureID;

	int							m_textureSlot;

	int							m_width, m_height, m_BPP;

	std::string					m_filePath;

	TextureType					m_textureType;

	unsigned char* m_localbuffer;
};

