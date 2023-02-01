#pragma once


/// <summary>
/// Base class interface for a Resource object
/// Mesh - Texture - Cubemap - Shader
/// </summary>
class IResource
{
public:
	IResource();
	~IResource();

	virtual void	Bind() = 0;
	virtual void	Unbind() = 0;

	virtual void	Parse(const std::string& filepath) = 0;
	virtual void	Parse(const std::string& firstFilepath, const std::string& secondFilepath) = 0;
	virtual void	Create() = 0;
	virtual void	Reset() = 0;

	bool			GetCreated() { return m_bIsCreated; }

protected:

	// Internal OpenGL index for resource
	OpenGLIndex		m_OpenGLResourceID;

	bool			m_bIsCreated;

	std::string		m_resourceFilePath;

};

