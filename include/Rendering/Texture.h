#pragma once

/// <summary>
/// Tells a Texture object which slot in the shader to use
/// Numbers correspond to index in the shader file
/// </summary>
enum class TextureType
{
	UNSET		= -1,
	DIFFUSE		= 0,
	SPECULAR	= 1,
	EMISSION	= 2,
	NORMAL		= 3,
	HEIGHT		= 4,
};

/// <summary>
/// Encapsulates an OpenGL texture buffer to act as a Texture
/// </summary>
class Texture
{
public:
	Texture();
	~Texture();

private:

	void			BindTexture();
	void			UnbindTexture();

	bool			ParseTexture(const std::string& filepath, TextureType textureType);
	void			CreateTexture();

	bool			GetIsCreated() { return m_bIsCreated; }

private:

	// OpenGL texture index
	uint32_t		m_textureOpenGLID;

	// Texture data
	int				m_textureWidth, m_textureHeight, m_textureBPP;

	std::string		m_textureFilePath;

	// Slot used in shader
	TextureType		m_textureType;

	// Temp storage during parsing and creation
	unsigned char*	m_pTempBuffer;

	bool			m_bIsCreated;

	// Ensures only the TextureManager can call functions of a texture object
	friend class	TextureManager;
};

