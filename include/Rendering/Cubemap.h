#pragma once
class Cubemap
{
public:
	Cubemap(const std::string& filepath);
	~Cubemap();

	void bindCubemap();
	void unbindCubemap();


private:

	uint32_t					m_texture = 0;

	int							m_textureSlot = 0;

	std::string					m_filePath;
	std::string					m_facesFilePath[6];

	int							m_width[6], m_height[6], m_BPP[6];

	enum cubeFaces
	{
		e_cubeFaceRight = 0,
		e_cubeFaceLeft = 1,
		e_cubeFaceTop = 2,
		e_cubeFaceBottom = 3,
		e_cubeFaceFront = 4,
		e_cubeFaceBack = 5,
		e_END_OF_CUBEFACE_ENUM
	};

	unsigned char* m_localbuffer[6];

};

