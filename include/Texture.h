#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>

// Stores texture information and provides a way to load a texture by interfacing with TextureManager
class Texture
{
public:
	Texture();
	~Texture();

	bool				loadTexture(const std::string& filePath);

	virtual void		Bind(unsigned int slot = 0) const;
	virtual void		Unbind() const;
	void				Unbind(unsigned int slot) const;

	const std::string&	getFilePath() const;
	const GLuint		getTexture() const;

protected:

	GLuint				m_texture;

	std::string			m_filePath;

	int					m_width, m_height, m_BPP;
};

//Stores cubemap information and provides a way to load a cubemap by interfacing with TextureManager. Only used for Skybox with fixed values for them
class CubeMap :
	public Texture
{
public:
	CubeMap();
	~CubeMap();

	void				Bind(unsigned int slot = 0) const override;
	void				Unbind() const override;

	bool				loadCubeMap(const std::string& filePath);

private:

	enum cubeFaces
	{
		e_cubeFaceRight		= 0,
		e_cubeFaceLeft		= 1,
		e_cubeFaceTop		= 2,
		e_cubeFaceBottom	= 3,
		e_cubeFaceFront		= 4,
		e_cubeFaceBack		= 5,
		e_CUBE_FACE_END		= 6
	};

	std::string m_cubeFaces[6];
};

//Stores current loaded textures ensuring only one of each texture gets loaded. Provides a way to load a texture from a file
class TextureManager
{
public:

	static std::shared_ptr<Texture> retrieveTexture(const std::string& filePath);
	static std::shared_ptr<CubeMap> retrieveCubeMap(const std::string& filePath = "");

	static void		clearTextures();
	static void		clearCubemaps();

private:

	static std::vector<std::shared_ptr<Texture>> m_loadedTextures;
	static std::vector<std::shared_ptr<CubeMap>> m_loadedCubemaps;

	TextureManager(); //Private so a class of this can't be initialized
};
