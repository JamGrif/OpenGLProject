#pragma once

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

class Cubemap
{
public:
	Cubemap();
	~Cubemap();

private:

	bool ParseCubemap(const CubemapFaces& facesFilepath);
	void CreateCubemap();

	void BindCubemap();
	void UnbindCubemap();

private:

	uint32_t					m_texture = 0;

	int							m_textureSlot = 0;

	//std::string					m_filePath;
	//std::string					m_facesFilePath[6];

	int							m_width[6], m_height[6], m_BPP[6];

	unsigned char* m_localbuffer[6];

	friend class CubemapManager;
};

