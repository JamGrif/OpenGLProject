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
	e_NormalShaderType = 0,
	e_TessellationShaderType = 1,
	e_GeometryShaderType = 2
};

// Stores shader information and provides a way to load a shader by interfacing with ShaderManager. 
// Can load Vertex shader, Fragment shader, Tessellation Control shader, Tessellation Evaluation shader & Geometry shader
class Shader :
	public BaseAsset
{
public:
	Shader();
	~Shader();

	inline void					readFromShaderFile();
	inline void					compileAndCreateShader();

	inline void					setFilePath(const std::string& vertexPath, const std::string& fragmentPath);
	inline void					setFilePath(const std::string& vertexPath, const std::string& tessellationControlPath, const std::string& tessellationEvaluationPath, const std::string& fragmentPath);
	inline void					setFilePath(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);


	void						Bind() const;
	void						Unbind() const;

	void						setUniform1i(const std::string& name, int v0);
	void						setUniform1f(const std::string& name, float v0);
	void						setUniform3f(const std::string& name, const glm::vec3& v0);
	void						setUniform4f(const std::string& name, const glm::vec4& v0);
	void						setUniformMatrix4fv(const std::string& name, const glm::mat4& v0);
	void						setUniformMatrix3fv(const std::string& name, const glm::mat3& v0);

	inline const GLuint			getProgram() const;
	inline const std::string&	getFilePath(int filePath) const;
	inline const int			getShaderType() const;

private:

	inline void					compileAndCreateNormalShader();
	inline void					compileAndCreateTesselationShader();
	inline void					compileAndCreateGeometryShader();

	inline const int			getUniformLocation(const std::string& name);

	GLuint						m_shaderProgram;

	int							m_shaderType;

	std::string					m_shaderFilePaths[5];

	std::string					m_shaderCode[5];

	//Cache for uniforms
	std::unordered_map<std::string, int>	m_locationCache;
};

//Stores current loaded shaders ensuring only one of each shaders gets loaded. Provides a way to load a shader from a file
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
