#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

//Stores texture information and provides a way to load a texture by interfacing with TextureManager
class Texture
{
public:
	Texture();
	~Texture();

	bool			loadTexture(std::string filePath);

	virtual void	Bind(unsigned int slot = 0) const;
	virtual void	Unbind() const;


	std::string		getFilePath() const;
	GLuint			getTexture() const;
	

protected:

	GLuint			m_texture;

	std::string		m_filePath;

	int				m_width, m_height, m_BPP;

};

//Stores cubemap information and provides a way to load a cubemap by interfacing with TextureManager. Only used for skybox with fixed values for them
class CubeMap :
	public Texture
	
{
public:
	CubeMap();
	~CubeMap();

	void			Bind(unsigned int slot = 0) const override;
	void			Unbind() const override;

	bool			loadCubeMap();

private:

	//Overwrite these files to import custom skybox
	std::vector<const char*> m_skyFaces =
	{ 
		"res/textures/sky/right.png" ,
		"res/textures/sky/left.png",
		"res/textures/sky/top.png",
		"res/textures/sky/bottom.png",
		"res/textures/sky/front.png",
		"res/textures/sky/back.png"
	};

};

//Stores current loaded textures ensuring only one of each texture gets loaded. Provides a way to load a texture from a file
class TextureManager
{
public:

	static Texture* retrieveTexture(std::string filePath);
	static CubeMap* retrieveCubeMap();

	static void		clearTextures();
	static void		clearCubemaps();

private:

	static std::vector<Texture*> m_loadedTextures;
	static std::vector<CubeMap*> m_loadedCubemaps;

	TextureManager(); //Private so a class of this can't be initalized
};

