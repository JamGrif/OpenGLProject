#pragma once

enum cubeFaces
{
	e_START_OF_CUBEFACE_ENUM = 0,
	e_cubeFaceRight = 0,
	e_cubeFaceLeft = 1,
	e_cubeFaceTop = 2,
	e_cubeFaceBottom = 3,
	e_cubeFaceFront = 4,
	e_cubeFaceBack = 5,
	e_END_OF_CUBEFACE_ENUM = 6
};

typedef std::array<std::string, 6> CubemapFaces;

class Cubemap
{
public:
	Cubemap(const CubemapFaces& facesFilepath);
	~Cubemap();

	void bindCubemap();
	void unbindCubemap();


private:

	uint32_t					m_texture = 0;

	int							m_textureSlot = 0;

	std::string					m_filePath;
	std::string					m_facesFilePath[6];

	int							m_width[6], m_height[6], m_BPP[6];

	

	unsigned char* m_localbuffer[6];

};

