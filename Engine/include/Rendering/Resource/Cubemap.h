#pragma once

#include "Rendering/Resource/IResource.h"

enum cubeFaces
{
	e_START_OF_CUBEFACE_ENUM	= 0,
	e_cubeFaceRight				= e_START_OF_CUBEFACE_ENUM,
	e_cubeFaceLeft,
	e_cubeFaceTop,
	e_cubeFaceBottom,
	e_cubeFaceFront,
	e_cubeFaceBack,
	e_END_OF_CUBEFACE_ENUM
};

typedef std::array<std::string, 6> CubemapFaces;

/// <summary>
/// Encapsulates an OpenGL texture buffer to act as a Cubemap
/// </summary>
class Cubemap :
	public IResource
{
public:
	Cubemap();
	~Cubemap();

private:

	virtual void	Parse(const std::string& facesFilepath);
	virtual void	Parse(const std::string& firstFilepath, const std::string& secondFilepath) {}

	virtual void	Create();

	virtual void	Bind();
	virtual void	Unbind();

	virtual void	Reset();

private:

	// Sampler2D slot in shader
	int32_t			m_textureSlot;

	// Cubemap face data
	int32_t			m_width[6], m_height[6], m_BPP[6];

	// Temp storage during parsing and creation
	unsigned char*	m_localbuffer[6];

	template<typename Cubemap>
	friend class ResourceManager;
};
