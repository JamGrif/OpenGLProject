#pragma once

// Everything that uses ShaderManager uses Shader
#include "Rendering/Shader.h"

/// <summary>
/// Abstracts and handles the creation, deletion and usage of shader objects
/// Shaders can only be used through this class
/// </summary>
class ShaderManager
{
public:

	bool parseShader(const std::string& shaderID, const std::string& vertexPath, const std::string& fragmentPath);

	void createAllShaders();

	Shader* getShaderAtID(const std::string& shaderID);

	void clearAllShaders();

	static ShaderManager* Instance() // Get instance
	{
		static ShaderManager* s_pInstance = new ShaderManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, Shader*> m_shaderMap;

	ShaderManager() {}
	~ShaderManager() {}
	ShaderManager(const ShaderManager&) = delete;
};
typedef ShaderManager TheShaderManager;
