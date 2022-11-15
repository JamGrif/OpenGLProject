#include "pch.h"
#include "Rendering/ShaderManager.h"

bool ShaderManager::parseShader(const std::string& shaderID, const std::string& vertexPath, const std::string& fragmentPath)
{
	// Check if material with ID already exists
	if (m_shaderMap.find(shaderID) != m_shaderMap.end())
		return false;

	//PRINT_TRACE("created shader with ID {0}", shaderID);

	Shader* pShader = new Shader();
	pShader->parseShader(vertexPath, fragmentPath);
	m_shaderMap.insert({ shaderID, pShader });

	return true;
}

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

void ShaderManager::clearAllShaders()
{
	//for (const auto& [key, value] : m_shaderMap)
	//{
	//	if (key != "screenbufferShader")
	//	{
	//		delete value;
	//	}
	//}

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
