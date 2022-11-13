#pragma once

#include <glm/mat4x4.hpp>

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void bindShader();
	void unbindShader();

	void							setUniform1i(const std::string& name, int v0);
	void							setUniform1f(const std::string& name, float v0);
	void							setUniform3f(const std::string& name, const glm::vec3& v0);
	void							setUniform4f(const std::string& name, const glm::vec4& v0);
	void							setUniformMatrix4fv(const std::string& name, const glm::mat4& v0);
	void							setUniformMatrix3fv(const std::string& name, const glm::mat3& v0);

private:
	// Drawing functions
	inline const int				getUniformLocation(const std::string& name);

	// Shader loading functions

	uint32_t						m_shaderProgram = 0;

	//shaderProgramType				m_shaderType;

	std::string						m_shaderFilePaths[5];

	std::string						m_shaderCode[5];

	// Cache for uniform locations
	std::unordered_map<std::string, int> m_locationCache;


};

