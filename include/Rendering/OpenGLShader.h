#pragma once

#include <glm/mat4x4.hpp>

#include <unordered_map>

/// <summary>
/// Used to indicate what type of shader program this shader object is
/// </summary>
enum class shaderProgramType
{
	e_NoProgramType			  = -1,
	e_NormalProgramType		  = 0, // vertex / fragment
	e_TessellationProgramType = 1, // vertex / tess control / tess evaluation / fragment
	e_GeometryProgramType	  = 2  // vertex / geometry / fragment
};

/// <summary>
/// Stores shader information and provides a way to load a shader by providing an interface for ShaderManager
/// Can load vertex, fragment, tessellation controls, tessellation evaluation & geometry shaders
/// </summary>
class OpenGLShader
{
public:
	OpenGLShader();
	~OpenGLShader();

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
	inline const shaderProgramType		getShaderType() const;

private:

	// Drawing functions
	inline const int			getUniformLocation(const std::string& name);

	// Shader loading functions

	uint32_t					m_shaderProgram;

	shaderProgramType			m_shaderType;

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

	static std::shared_ptr<OpenGLShader>	retrieveShader(const char* vertexPath, const char* fragmentPath);
	static std::shared_ptr<OpenGLShader>	retrieveShader(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath);
	static std::shared_ptr<OpenGLShader>	retrieveShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

	static void						readShadersFromFile();
	static void						createShaders();

	static void						clearShaders();

private:

	static std::vector<std::shared_ptr<OpenGLShader>>	m_loadedShaders;

	ShaderManager(); //Private so a class of this can't be initialized
};
