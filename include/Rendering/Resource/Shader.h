#pragma once

#include "Rendering/Resource/IResource.h"

#include <glm/mat4x4.hpp>

class Shader :
	public IResource
{
public:
	Shader();
	~Shader();

	void			SetUniform(const std::string& uniformName, int value);
	void			SetUniform(const std::string& uniformName, float value);
	void			SetUniform(const std::string& uniformName, const glm::vec3& value);
	void			SetUniform(const std::string& uniformName, const glm::vec4& value);
	void			SetUniform(const std::string& uniformName, const glm::mat3& value);
	void			SetUniform(const std::string& uniformName, const glm::mat4& value);

private:

	virtual void Parse(const std::string& filepath) {}
	virtual void Parse(const std::string& vertexPath, const std::string& fragmentPath);
	virtual void Create();

	virtual void Bind();
	virtual void Unbind();

	virtual void Reset();


	inline int		GetUniformLocation(const std::string& uniformName);

	//bool			GetCreated() { return m_bCreated; }

private:

	//uint32_t						m_shaderProgram;

	//bool							m_bCreated;

	// Cache for uniform locations
	std::unordered_map<std::string, int> m_uniformLocationCache;

	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	
	template<typename Shader>
	friend class ResourceManager;
};
