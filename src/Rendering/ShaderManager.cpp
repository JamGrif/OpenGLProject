#include "pch.h"
#include "Rendering/ShaderManager.h"

/// <summary>
/// Parse the specified shader at vertex and fragment path and add it to the shaderMap
/// IMPORTANT: When all shaders have been parsed, createAllShaders() must be called before shaders can be used
/// </summary>
bool ShaderManager::parseShader(const std::string& shaderID, const std::string& vertexPath, const std::string& fragmentPath)
{
	// Check if material with ID already exists
	if (m_shaderMap.find(shaderID) != m_shaderMap.end())
		return false;

	// Create shader object, parse it and add to shaderMap
	Shader* pShader = new Shader();
	pShader->parseShader(vertexPath, fragmentPath);
	m_shaderMap.insert({ shaderID, pShader });

	return true;
}

/// <summary>
/// Create all OpenGL shaders of successfully parsed shader objects
/// </summary>
void ShaderManager::createAllShaders()
{
	for (const auto& [key, value] : m_shaderMap)
	{
		value->createShader();
	}
}

Shader* ShaderManager::getShaderAtID(const std::string& shaderID)
{
	return m_shaderMap.find(shaderID) != m_shaderMap.end() ? m_shaderMap.at(shaderID) : nullptr;
}

/// <summary>
/// Clear all shader objects, except the screenbuffer shader
/// </summary>
void ShaderManager::clearAllShaders()
{
	// Make sure to not delete screenbuffer shader
	auto it = m_shaderMap.begin();
	while (it != m_shaderMap.end())
	{
		if (it->first == "screenbufferShader")
		{
			it++;
		}
		else
		{
			delete it->second;
			it = m_shaderMap.erase(it);
		}
	}
}
