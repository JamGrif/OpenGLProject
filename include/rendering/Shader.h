#pragma once

#include "rendering/BaseAsset.h"

#include <unordered_map>

enum shaderFilePath
{
	e_VertexShader = 0,
	e_FragmentShader = 1,
	e_TessellationControlShader = 2,
	e_TessellationEvaluationShader = 3,
	e_GeometryShader = 4
};

enum ShaderType
{
	e_NoShaderType = -1,
	e_NormalShaderType = 0,
	e_TessellationShaderType = 1,
	e_GeometryShaderType = 2
};

/// <summary>
/// Stores shader information and provides a way to load a shader by providing an interface for ShaderManager
/// Can load vertex, fragment, tessellation controls, tessellation evaluation & geometry shaders
/// </summary>
class Shader :
	public BaseAsset
{
public:
	Shader();
	~Shader();

	// Drawing functions
	void						Bind() const;
	void						Unbind() const;

	void						setUniform1i(const std::string& name, int v0);
	void						setUniform1f(const std::string& name, float v0);
	void						setUniform3f(const std::string& name, const glm::vec3& v0);
	void						setUniform4f(const std::string& name, const glm::vec4& v0);
	void						setUniformMatrix4fv(const std::string& name, const glm::mat4& v0);
	void						setUniformMatrix3fv(const std::string& name, const glm::mat3& v0);

	// Shader loading functions
	void						readSourceCodeFromFile();
	void						createShaderProgram();

	inline void					setFilePath(const std::string& vertexPath, const std::string& fragmentPath);
	inline void					setFilePath(const std::string& vertexPath, const std::string& tessellationControlPath, const std::string& tessellationEvaluationPath, const std::string& fragmentPath);
	inline void					setFilePath(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);

	inline const std::string&	getFilePath(int filePath) const;
	inline const int			getShaderType() const;

private:

	// Drawing functions
	inline const int			getUniformLocation(const std::string& name);

	// Shader loading functions

	GLuint						m_shaderProgram;

	ShaderType					m_shaderType;

	std::string					m_shaderFilePaths[5];

	std::string					m_shaderCode[5];

	// Cache for uniform locations
	std::unordered_map<std::string, int>	m_locationCache;
};



/// <summary>
/// Stores created shader objects, only one version of a shader object will exist for each filepath set.
/// Do all shader creation and deleting through the TextureManager
/// </summary>
class ShaderManager
{
public:

	static std::shared_ptr<Shader>	retrieveShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	static std::shared_ptr<Shader>	retrieveShader(const GLchar* vertexPath, const GLchar* tessellationControlPath, const GLchar* tessellationEvaluationPath, const GLchar* fragmentPath);
	static std::shared_ptr<Shader>	retrieveShader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);

	static void						readShadersFromFile();
	static void						createShaders();

	static void						clearShaders();

private:

	static std::vector<std::shared_ptr<Shader>>	m_loadedShaders;

	ShaderManager(); //Private so a class of this can't be initialized
};
