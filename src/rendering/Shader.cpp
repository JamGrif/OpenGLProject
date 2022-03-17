#include "rendering/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::shared_ptr<Shader>> ShaderManager::m_loadedShaders;

Shader::Shader()
	:m_shaderProgram(0), m_vertexPath(""), m_fragmentPath(""), m_tessellationControlPath(""), m_tessellationEvaluationPath(""), m_geometryPath("")
{
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderProgram);
}

void Shader::loadShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	m_vertexPath = vertexPath;
	m_fragmentPath = fragmentPath;

	//Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//Close File handlers
		vShaderFile.close();
		fShaderFile.close();

		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	//Compile our shader program
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	//Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, fragment);
	glLinkProgram(m_shaderProgram);

	//Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete the shaders as they're linked into our program now and no longer neccessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::loadShader(const GLchar* vertexPath, const GLchar* tessellationControlPath, const GLchar* tessellationEvaluationPath, const GLchar* fragmentPath)
{
	m_vertexPath = vertexPath;
	m_tessellationControlPath = tessellationControlPath;
	m_tessellationEvaluationPath = tessellationEvaluationPath;
	m_fragmentPath = fragmentPath;

	//Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string tcCode;
	std::string teCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream tcShaderFile;
	std::ifstream teShaderFile;
	

	//Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	tcShaderFile.exceptions(std::ifstream::badbit);
	teShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		tcShaderFile.open(tessellationControlPath);
		teShaderFile.open(tessellationEvaluationPath);
		std::stringstream vShaderStream, fShaderStream, tcShaderStream, teShaderStream;

		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		tcShaderStream << tcShaderFile.rdbuf();
		teShaderStream << teShaderFile.rdbuf();

		//Close File handlers
		vShaderFile.close();
		fShaderFile.close();
		tcShaderFile.close();
		teShaderFile.close();

		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		teCode = teShaderStream.str();
		tcCode = tcShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* tcShaderCode = tcCode.c_str();
	const GLchar* teShaderCode = teCode.c_str();

	//Compile our shader program
	GLuint vertex, fragment, TControl, TEvaluation;
	GLint success;
	GLchar infoLog[512];

	//Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Tessellation Control
	TControl = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(TControl, 1, &tcShaderCode, NULL);
	glCompileShader(TControl);

	glGetShaderiv(TControl, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(TControl, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Tessellation Evaluation
	TEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(TEvaluation, 1, &teShaderCode, NULL);
	glCompileShader(TEvaluation);

	glGetShaderiv(TEvaluation, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(TEvaluation, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, TControl);
	glAttachShader(m_shaderProgram, TEvaluation);
	glAttachShader(m_shaderProgram, fragment);
	glLinkProgram(m_shaderProgram);

	//Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete the shaders as they're linked into our program now and no longer neccessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(TControl);
	glDeleteShader(TEvaluation);
}

void Shader::loadShader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath)
{
	m_vertexPath = vertexPath;
	m_geometryPath = geometryPath;
	m_fragmentPath = fragmentPath;

	//Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geoCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream geoShaderFile;


	//Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	geoShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		geoShaderFile.open(geometryPath);
		std::stringstream vShaderStream, fShaderStream, geoShaderStream;

		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		geoShaderStream << geoShaderFile.rdbuf();

		//Close File handlers
		vShaderFile.close();
		fShaderFile.close();
		geoShaderFile.close();

		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geoCode = geoShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* geoShaderCode = geoCode.c_str();

	//Compile our shader program
	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];

	//Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Geometry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &geoShaderCode, NULL);
	glCompileShader(geometry);

	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(geometry, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	//Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	//Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, geometry);
	glAttachShader(m_shaderProgram, fragment);
	glLinkProgram(m_shaderProgram);

	//Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete the shaders as they're linked into our program now and no longer neccessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

/// <summary>
/// Binds the shader to the rendering pipeline
/// </summary>
void Shader::Bind() const
{
	glUseProgram(m_shaderProgram);
}

/// <summary>
/// Unbinds the shader to the rendering pipeline
/// </summary>
void Shader::Unbind() const
{
	glUseProgram(0);
}

/// <summary>
/// Sets 1 int uniform value
/// </summary>
/// <param name="name">Name of uniform in shader</param>
/// <param name="v0">Value of int</param>
void Shader::setUniform1i(const std::string& name, int v0) 
{
	glUniform1i(getUniformLocation(name), v0);
}

/// <summary>
/// Sets 1 float uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of float</param>
void Shader::setUniform1f(const std::string& name, float v0) 
{
	glUniform1f(getUniformLocation(name), v0);
}

/// <summary>
/// Sets 3 float uniform values
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the float 3</param>
void Shader::setUniform3f(const std::string& name, const glm::vec3& v0) 
{
	glUniform3f(getUniformLocation(name), v0.x, v0.y, v0.z);
}

/// <summary>
/// Sets 4 float uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the float 4</param>
void Shader::setUniform4f(const std::string& name, const glm::vec4& v0) 
{
	glUniform4f(getUniformLocation(name), v0.x, v0.y, v0.z, v0.w);
}

/// <summary>
/// Sets a 4x4 matrix uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the 4x4 matrix</param>
void Shader::setUniformMatrix4fv(const std::string& name, const glm::mat4& v0) 
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0));
}

/// <summary>
/// Sets a 3x3 matrix uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the 4x4 matrix</param>
void Shader::setUniformMatrix3fv(const std::string& name, const glm::mat3& v0)
{
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0));
}

const GLuint Shader::getProgram() const
{
	return m_shaderProgram;
}

const GLchar* Shader::getFilePath(shaderFilePath filePath) const
{
	switch (filePath)
	{
	case e_VertexPath:
		return m_vertexPath;

	case e_FragmentPath:
		return m_fragmentPath;

	case e_TessellationControlPath:
		return m_tessellationControlPath;

	case e_TessellationEvaluationPath:
		return m_tessellationEvaluationPath;

	case e_GeometryPath:
		return m_geometryPath;
	}
	return nullptr;
}

/// <summary>
/// Cache system that only finds the location of a uniform once and then stores it location.
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <returns>Location of the uniform value</returns>
const int Shader::getUniformLocation(const std::string& name) 
{
	if (m_locationCache.find(name) != m_locationCache.end())
	{
		return m_locationCache[name];
	}

	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "SHADER->Uniform location " << name << " does not exist!" << std::endl;
	}
	m_locationCache[name] = location;
	
	return location;
}

/// <summary>
/// Loads the specified shader, if shader already exists it returns a pointer to it instead of reloading the same shader
/// </summary>
/// <param name="vertexPath">Vertex shader file path</param>
/// <param name="fragmentPath">Fragment shader file path</param>
/// <returns>Pointer to the created shader</returns>
std::shared_ptr<Shader> ShaderManager::retrieveShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	//Check if shader program is already loaded
	for (std::shared_ptr<Shader> s : m_loadedShaders)
	{
		if (s->getFilePath(e_VertexPath) == vertexPath && s->getFilePath(e_FragmentPath) == fragmentPath)
		{
			return s;
		}
	}

	//Otherwise, create new texture and add it to vector
	std::cout << "SHADERMANAGER->" << vertexPath << " + " << fragmentPath << " program is being created" << std::endl;

	std::shared_ptr<Shader> s = std::make_shared<Shader>();
	s->loadShader(vertexPath, fragmentPath);
	m_loadedShaders.push_back(s);
	return s;
}

std::shared_ptr<Shader> ShaderManager::retrieveShader(const GLchar* vertexPath, const GLchar* tessellationControlPath, const GLchar* tessellationEvaluationPath, const GLchar* fragmentPath)
{
	//Check if exact same shader program is already loaded
	for (std::shared_ptr<Shader> s : m_loadedShaders)
	{
		if (s->getFilePath(e_VertexPath) == vertexPath &&
			s->getFilePath(e_TessellationControlPath) == tessellationControlPath &&
			s->getFilePath(e_TessellationEvaluationPath) == tessellationEvaluationPath &&
			s->getFilePath(e_FragmentPath) == fragmentPath)
		{
			return s;
		}
	}

	//Otherwise, create new texture and add it to vector
	std::cout << "SHADERMANAGER->" << vertexPath << " + " << fragmentPath << " program is being created" << std::endl;

	std::shared_ptr<Shader> s = std::make_shared<Shader>();
	s->loadShader(vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);
	m_loadedShaders.push_back(s);
	return s;
}

std::shared_ptr<Shader> ShaderManager::retrieveShader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath)
{
	//Check if exact same shader program is already loaded
	for (std::shared_ptr<Shader> s : m_loadedShaders)
	{
		if (s->getFilePath(e_VertexPath) == vertexPath &&
			s->getFilePath(e_GeometryPath) == geometryPath &&
			s->getFilePath(e_FragmentPath) == fragmentPath)
		{
			//std::cout << "SHADERMANAGER->" << vertexPath << " + " << fragmentPath << " program already exists, returning loaded shader program" << std::endl;
			return s;
		}
	}

	//Otherwise, create new texture and add it to vector
	std::cout << "SHADERMANAGER->" << vertexPath << " + " << fragmentPath << " program is being created" << std::endl;

	std::shared_ptr<Shader> s = std::make_shared<Shader>();
	s->loadShader(vertexPath, geometryPath, fragmentPath);
	m_loadedShaders.push_back(s);
	return s;
}

void ShaderManager::clearShaders()
{
	m_loadedShaders.clear();
}
