#pragma once

#include "Rendering/Resource/IResource.h"

/// <summary>
/// Tells a Texture object which slot in the shader to use
/// Numbers correspond to index in the shader file
/// </summary>
enum class TextureType
{
	UNSET					= -1,
	START_OF_TEXTURETYPE	= 0,
	DIFFUSE					= START_OF_TEXTURETYPE,
	SPECULAR				= 1,
	EMISSION				= 2,
	NORMAL					= 3,
	HEIGHT					= 4,
	END_OF_TEXTURETYPE
};

/// <summary>
/// Encapsulates an OpenGL texture buffer to act as a Texture
/// </summary>
class Texture :
	public IResource
{
public:
	Texture();
	~Texture();

	void			setTextureType(TextureType type) { m_textureType = type; }
private:

	virtual void	Bind() override;
	virtual void	Unbind() override;

	virtual void	Parse(const std::string& filepath) override;
	virtual void	Parse(const std::string& firstFilepath, const std::string& secondFilepath) {}
	virtual void	Create() override;

	virtual void	Reset();

private:

	// Texture data
	int32_t			m_textureWidth, m_textureHeight, m_textureBPP;

	// sampler2D slot used in shader
	TextureType		m_textureType;

	// Temp storage during parsing and creation
	unsigned char*	m_pTempBuffer;

	// Ensures only the TextureManager can call functions of a texture object
	template<typename Texture>
	friend class ResourceManager;
};
