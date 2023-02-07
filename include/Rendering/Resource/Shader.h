#pragma once

#include "Rendering/Resource/IResource.h"

#include <glm/mat4x4.hpp>

/// <summary>
/// Encapsulates an OpenGL program to act as a shader
/// </summary>
class Shader :
	public IResource
{
public:
	Shader();
	~Shader();

	void			SetUniform(const std::string& uniformName, int value);
	void			SetUniform(const std::string& uniformName, float value);
	void			SetUniform(const std::string& uniformName, const Vector3D& value);
	void			SetUniform(const std::string& uniformName, const glm::vec4& value);
	void			SetUniform(const std::string& uniformName, const glm::mat3& value);
	void			SetUniform(const std::string& uniformName, const glm::mat4& value);

private:

	virtual void	Parse(const std::string& filepath) {}
	virtual void	Parse(const std::string& vertexPath, const std::string& fragmentPath);
	virtual void	Create();

	virtual void	Bind();
	virtual void	Unbind();

	virtual void	Reset();

	inline int		GetUniformLocation(const std::string& uniformName);

private:

	std::string		vertexShaderCode;
	std::string		fragmentShaderCode;

	typedef std::unordered_map<std::string, uint32_t> UniformCache;
	UniformCache	m_uniformLocationCache;

	template<typename Shader>
	friend class ResourceManager;
};
