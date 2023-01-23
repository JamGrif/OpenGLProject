#include "pch.h"
#include "Rendering/Resource/Manager/ShaderManager.h"

/// <summary>
/// Parse the specified shader at vertex and fragment path and add it to the shaderMap
/// IMPORTANT: When all shaders have been parsed, createAllShaders() must be called before shaders can be used
/// </summary>
bool ShaderManager::AddShader(const std::string& shaderID, const std::string& vertexPath, const std::string& fragmentPath)
{
	// Check if material with ID already exists
	if (m_shaderMap.find(shaderID) != m_shaderMap.end())
		return false;

	// Create shader object, parse it and add to shaderMap
	//Shader* pShader = new Shader();
	std::unique_ptr<Shader> pShader = std::make_unique<Shader>();
	pShader->ParseShader(vertexPath, fragmentPath);
	m_shaderMap.insert({ shaderID, std::move(pShader) });

	return true;
}

/// <summary>
/// Create all OpenGL shaders that have been parsed
/// </summary>
void ShaderManager::CreateAllShaders()
{
	for (const auto& [key, value] : m_shaderMap)
	{
		if (!value->GetCreated())
			value->CreateShader();
	}
}

void ShaderManager::BindShaderAtID(const std::string& shaderID)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->BindShader();
	}
}

void ShaderManager::UnbindShaderAtID(const std::string& shaderID)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->UnbindShader();
	}
}

void ShaderManager::SetUniformAtID(const std::string& shaderID, const std::string& uniformName, int value)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->SetUniform(uniformName, value);
	}
}

void ShaderManager::SetUniformAtID(const std::string& shaderID, const std::string& uniformName, float value)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->SetUniform(uniformName, value);
	}
}

void ShaderManager::SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::vec3& value)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->SetUniform(uniformName, value);
	}
}

void ShaderManager::SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::vec4& value)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->SetUniform(uniformName, value);
	}
}

void ShaderManager::SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::mat3& value)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->SetUniform(uniformName, value);
	}
}

void ShaderManager::SetUniformAtID(const std::string& shaderID, const std::string& uniformName, const glm::mat4& value)
{
	if (m_shaderMap.count(shaderID))
	{
		m_shaderMap.at(shaderID)->SetUniform(uniformName, value);
	}
}

/// <summary>
/// Clear all shader objects, except the screenbuffer shader
/// </summary>
void ShaderManager::ClearAllShaders()
{
	auto it = m_shaderMap.begin();
	while (it != m_shaderMap.end())
	{
		if (it->first == "screenbufferShader")
			it++;
		else
			it = m_shaderMap.erase(it);
	}
}
