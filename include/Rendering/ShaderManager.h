#pragma once

// Everything that uses ShaderManager uses Shader
#include "Rendering/Shader.h"

class ShaderManager
{
public:

	bool parseShader(const std::string& shaderID, const std::string& vertexPath, const std::string& fragmentPath);

	void createAllShaders();

	Shader* getShaderAtID(const std::string& shaderID);

	void clearAllShaders();

	static ShaderManager* Instance()
	{
		static ShaderManager* s_pInstance = new ShaderManager;
		return s_pInstance;
	}
private:

	std::unordered_map<std::string, Shader*> m_shaderMap;

	ShaderManager() {}
	~ShaderManager() {}

};
typedef ShaderManager TheShaderManager;
