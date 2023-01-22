#pragma once

// Everything that uses ShaderManager uses Shader
#include "Rendering/Shader.h"

/// <summary>
/// Encapsulates and abstracts the creation, deletion and usage of shader objects
/// Shaders can only be used through this class
/// </summary>
class ShaderManager
{
public:

	bool AddShader(const std::string& shaderID, const std::string& vertexPath, const std::string& fragmentPath);

	void CreateAllShaders();

	void BindShaderAtID(const std::string& shaderID);
	void UnbindShaderAtID(const std::string& shaderID);

	void SetUniformAtID(const std::string& shaderID, const std::string& uniformName, int value);
	void SetUniformAtID(const std::string& shaderID, const std::string& uniformName, float value);
	void SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::vec3& value);
	void SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::vec4& value);
	void SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::mat3& value);
	void SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::mat4& value);

	void ClearAllShaders();

	static ShaderManager* Instance() // Get instance
	{
		static ShaderManager* s_pInstance = new ShaderManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaderMap;

	ShaderManager() {}
	~ShaderManager() {}
	ShaderManager(const ShaderManager&) = delete;
};
typedef ShaderManager TheShaderManager;
