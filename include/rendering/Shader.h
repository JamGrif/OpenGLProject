#pragma once

#include "rendering/BaseAsset.h"

#include <unordered_map>

enum shaderFilePath
{
	e_VertexPath = 0,
	e_FragmentPath = 1,
	e_TessellationControlPath = 2,
	e_TessellationEvaluationPath = 3,
	e_GeometryPath = 4
};

// Stores shader information and provides a way to load a shader by interfacing with ShaderManager. 
// Can load Vertex shader, Fragment shader, Tessellation Control shader, Tessellation Evaluation shader & Geometry shader
class Shader :
	public BaseAsset
{
public:
	Shader();
	~Shader();

	void					loadShader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void					loadShader(const GLchar* vertexPath, const GLchar* tessellationControlPath, const GLchar* tessellationEvaluationPath, const GLchar* fragmentPath);
	void					loadShader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);

	void					Bind() const;
	void					Unbind() const;

	void					setUniform1i(const std::string& name, int v0);
	void					setUniform1f(const std::string& name, float v0);
	void					setUniform3f(const std::string& name, const glm::vec3& v0);
	void					setUniform4f(const std::string& name, const glm::vec4& v0);
	void					setUniformMatrix4fv(const std::string& name, const glm::mat4& v0);
	void					setUniformMatrix3fv(const std::string& name, const glm::mat3& v0);

	const GLuint			getProgram() const;
	const GLchar*			getFilePath(shaderFilePath filePath) const;

private:

	const int				getUniformLocation(const std::string& name);

	GLuint					m_shaderProgram;

	const GLchar*			m_vertexPath;
	const GLchar*			m_fragmentPath;
	const GLchar*			m_tessellationEvaluationPath;
	const GLchar*			m_tessellationControlPath;
	const GLchar*			m_geometryPath;

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

	static void						clearShaders();

private:

	static std::vector<std::shared_ptr<Shader>>	m_loadedShaders;

	ShaderManager(); //Private so a class of this can't be initialized
};
