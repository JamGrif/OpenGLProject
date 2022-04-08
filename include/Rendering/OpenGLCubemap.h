#pragma once

// Stores cubemap information and provides a way to load a cubemap by interfacing with TextureManager. Only used for Skybox with fixed values for them
class OpenGLCubemap
{
public:
	OpenGLCubemap();
	~OpenGLCubemap();

	void						readCubemapFromFile();
	inline bool					loadCubemap();

	void				Bind() const;
	void				Unbind() const;

	void						setTextureSlot(int slot);

	inline void					setFilePath(const std::string& filePath);

	inline const std::string&	getFilePath() const;

private:

	uint32_t					m_texture;

	int							m_textureSlot;

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

// Stores currently loaded cubemaps ensuring only one of each texture gets loaded. Provides a way to load a texture from a file
class CubemapManager
{
public:

	static std::shared_ptr<OpenGLCubemap> retrieveCubeMapObject(const std::string& filePath = "");

	static void     readCubemapsFromFile();
	static void		createCubemaps();

	static void		clearCubemaps();

private:

	static std::vector<std::shared_ptr<OpenGLCubemap>> m_loadedCubemaps;

	CubemapManager(); // Private so a class of this can't be initialized
};

