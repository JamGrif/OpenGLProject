#pragma once

/// <summary>
/// Tells a Texture object which slot in the shader to use
/// Numbers correspond to numbers in the shader file
/// </summary>
enum class TextureType
{
	UNSET_TEXTURE		= -1,
	DIFFUSE_TEXTURE		= 0,
	SPECULAR_TEXTURE	= 1,
	EMISSION_TEXTURE	= 2,
	NORMAL_TEXTURE		= 3,
	HEIGHT_TEXTURE		= 4,
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

	void bindTexture();
	void unbindTexture();

	bool parseTexture(const std::string& filepath, TextureType textureType);
	void createTexture();

private:

	// Identifies to OpenGL which texture object this is
	uint32_t			m_textureOpenGLID;

	int					m_textureWidth, m_textureHeight, m_textureBPP;

	std::string			m_textureFilePath;

	// Used to decide what slot in shader to use
	TextureType			m_textureType;

	// Temp storage of texture during creation
	unsigned char*		m_pLocalTempBuffer;

	// Ensures only the TextureManager can call functions of a texture object
	friend class TextureManager;
};

