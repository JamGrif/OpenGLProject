#pragma once

#include <glm/mat4x4.hpp>

class Shader
{
public:
	Shader();
	~Shader();

private:

	void ParseShader(const std::string& vertexPath, const std::string& fragmentPath);
	void CreateShader();

	void BindShader();
	void UnbindShader();

	void			SetUniform(const std::string& uniformName, int value);
	void			SetUniform(const std::string& uniformName, float value);
	void			SetUniform(const std::string& uniformName, const glm::vec3& value);
	void			SetUniform(const std::string& uniformName, const glm::vec4& value);
	void			SetUniform(const std::string& uniformName, const glm::mat3& value);
	void			SetUniform(const std::string& uniformName, const glm::mat4& value);

	inline int		GetUniformLocation(const std::string& uniformName);

	bool			GetCreated() { return m_bCreated; }

private:

	uint32_t						m_shaderProgram;

	bool							m_bCreated;

	// Cache for uniform locations
	std::unordered_map<std::string, int> m_uniformLocationCache;

	std::string vertexShaderCode;
	std::string fragmentShaderCode;

	friend class ShaderManager;
};
