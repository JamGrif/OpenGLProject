#include "rendering/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::shared_ptr<Shader>> ShaderManager::m_loadedShaders;

Shader::Shader()
	:m_shaderProgram(0), m_shaderType(-1)
{
}

Shader::~Shader()
{
	glDeleteProgram(m_shaderProgram);
}

/// <summary>
/// Binds the shader
/// </summary>
void Shader::Bind() const
{
	glUseProgram(m_shaderProgram);
}

/// <summary>
/// Unbinds the shader
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

/// <summary>
/// Uses the saved filepath (from .setFilePath()) to load the read the shader textfile into the object.
/// Must be called BEFORE .loadShader()
/// </summary>
void Shader::readShaderFromFile()
{
	// All shader types will have both a vertex and fragment shader
	
	// Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

	try
	{
		// Open files
		vShaderFile.open(m_shaderFilePaths[e_VertexShader]);
		fShaderFile.open(m_shaderFilePaths[e_FragmentShader]);
		std::stringstream vShaderStream, fShaderStream;

		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close File handlers
		vShaderFile.close();
		fShaderFile.close();

		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	m_shaderCode[e_VertexShader] = vertexCode.c_str();
	m_shaderCode[e_FragmentShader] = fragmentCode.c_str();

	if (m_shaderType == e_NormalShaderType)
	{
		return;
	}

	// If a tessellation shader, also read and save from the tessellation shader text files
	if (m_shaderType == e_TessellationShaderType)
	{
		// Retrieve the vertex/fragment source code from filePath

		std::string tcCode;
		std::string teCode;

		std::ifstream tcShaderFile;
		std::ifstream teShaderFile;

		tcShaderFile.exceptions(std::ifstream::badbit);
		teShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			// Open files
			tcShaderFile.open(m_shaderFilePaths[e_TessellationControlShader]);
			teShaderFile.open(m_shaderFilePaths[e_TessellationEvaluationShader]);
			std::stringstream tcShaderStream, teShaderStream;

			// Read file's buffer contents into streams
			tcShaderStream << tcShaderFile.rdbuf();
			teShaderStream << teShaderFile.rdbuf();

			// Close File handlers
			tcShaderFile.close();
			teShaderFile.close();

			// Convert stream into string
			teCode = teShaderStream.str();
			tcCode = tcShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		m_shaderCode[e_TessellationControlShader] = tcCode.c_str();
		m_shaderCode[e_TessellationEvaluationShader] = teCode.c_str();

		return;
	}

	// If a geometry shader, also read and save from the tessellation shader text files
	if (m_shaderType == e_GeometryShaderType)
	{
		// Retrieve the vertex/fragment source code from filePath
		std::string geoCode;

		std::ifstream geoShaderFile;


		// Ensure ifstream objects can throw exceptions
		geoShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			// Open files
			geoShaderFile.open(m_shaderFilePaths[e_GeometryShader]);
			std::stringstream geoShaderStream;

			// Read file's buffer contents into streams
			geoShaderStream << geoShaderFile.rdbuf();

			// Close File handlers
			geoShaderFile.close();

			// Convert stream into string
			geoCode = geoShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		m_shaderCode[e_GeometryShader] = geoCode.c_str();
	}
}

/// <summary>
/// Turns the saved shader code into a OpenGL shader program
/// Must be called after .readFromShaderFile() loadShader
/// </summary>
void Shader::loadShader()
{
	if (m_shaderType == e_NormalShaderType)
	{
		compileAndCreateNormalShader();
	}
	else if (m_shaderType == e_TessellationShaderType)
	{
		compileAndCreateTesselationShader();
	}
	else if (m_shaderType == e_GeometryShaderType)
	{
		compileAndCreateGeometryShader();
	}
}

/// <summary>
/// NORMAL VERTEX/FRAGMENT SHADER ONLY - Sets the shader type to be a normal shader and saves the file paths for the specific shader files 
/// </summary>
void Shader::setFilePath(const std::string& vertexPath, const std::string& fragmentPath)
{
	m_shaderType = e_NormalShaderType;

	m_shaderFilePaths[e_VertexShader] = vertexPath;
	m_shaderFilePaths[e_FragmentShader] = fragmentPath;
}

/// <summary>
/// TESSELLATION SHADER ONLY - Sets the shader type to be a tessellation shader and saves the file paths for the specific shader files 
/// </summary>
void Shader::setFilePath(const std::string& vertexPath, const std::string& tessellationControlPath, const std::string& tessellationEvaluationPath, const std::string& fragmentPath)
{
	m_shaderType = e_TessellationShaderType;

	m_shaderFilePaths[e_VertexShader] = vertexPath;
	m_shaderFilePaths[e_TessellationControlShader] = tessellationControlPath;
	m_shaderFilePaths[e_TessellationEvaluationShader] = tessellationEvaluationPath;
	m_shaderFilePaths[e_FragmentShader] = fragmentPath;
}

/// <summary>
/// GEOMETRY SHADER ONLY - Sets the shader type to be a geometry shader and saves the file paths for the specific shader files 
/// </summary>
void Shader::setFilePath(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath)
{
	m_shaderType = e_GeometryShaderType;

	m_shaderFilePaths[e_VertexShader] = vertexPath;
	m_shaderFilePaths[e_GeometryShader] = geometryPath;
	m_shaderFilePaths[e_FragmentShader] = fragmentPath;
}


/// <summary>
/// Returns the filepath to a specific shader file
/// </summary>
const std::string& Shader::getFilePath(int filePath) const
{
	switch (filePath)
	{
	case e_VertexShader:
		return m_shaderFilePaths[e_VertexShader];

	case e_FragmentShader:
		return m_shaderFilePaths[e_FragmentShader];

	case e_TessellationControlShader:
		return m_shaderFilePaths[e_TessellationControlShader];

	case e_TessellationEvaluationShader:
		return m_shaderFilePaths[e_TessellationEvaluationShader];

	case e_GeometryShader:
		return m_shaderFilePaths[e_GeometryShader];

	}
}

/// <summary>
/// Returns if the shader is a normal, tessellation or geometry shader
/// </summary>
/// <returns></returns>
const int Shader::getShaderType() const
{
	return m_shaderType;
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
/// Compiles the vertex & fragment code into a single shader program
/// </summary>
void Shader::compileAndCreateNormalShader()
{
	// Compile our shader program
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	const GLchar* testvertex = m_shaderCode[e_VertexShader].c_str();
	const GLchar* testfragment = m_shaderCode[e_FragmentShader].c_str();

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &testvertex, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &testfragment, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, fragment);
	glLinkProgram(m_shaderProgram);

	// Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

/// <summary>
/// Compiles the vertex, tessellation control, tessellation evaluation & fragment code into a single shader program
/// </summary>
void Shader::compileAndCreateTesselationShader()
{
	const GLchar* testvertex = m_shaderCode[e_VertexShader].c_str();
	const GLchar* testfragment = m_shaderCode[e_FragmentShader].c_str();
	const GLchar* testtcontrol = m_shaderCode[e_TessellationControlShader].c_str();
	const GLchar* testtevaluation = m_shaderCode[e_TessellationEvaluationShader].c_str();

	// Compile our shader program
	GLuint vertex, fragment, TControl, TEvaluation;
	GLint success;
	GLchar infoLog[512];

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &testvertex, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Tessellation Control
	TControl = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(TControl, 1, &testtcontrol, NULL);
	glCompileShader(TControl);

	glGetShaderiv(TControl, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(TControl, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// Tessellation Evaluation
	TEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(TEvaluation, 1, &testtevaluation, NULL);
	glCompileShader(TEvaluation);

	glGetShaderiv(TEvaluation, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(TEvaluation, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &testfragment, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, TControl);
	glAttachShader(m_shaderProgram, TEvaluation);
	glAttachShader(m_shaderProgram, fragment);
	glLinkProgram(m_shaderProgram);

	// Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(TControl);
	glDeleteShader(TEvaluation);
}

/// <summary>
/// Compiles the vertex, geometry & fragment code into a single shader program
/// </summary>
void Shader::compileAndCreateGeometryShader()
{
	const GLchar* testvertex = m_shaderCode[e_VertexShader].c_str();
	const GLchar* testfragment = m_shaderCode[e_FragmentShader].c_str();
	const GLchar* testgeometry = m_shaderCode[e_GeometryShader].c_str();

	// Compile our shader program
	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &testvertex, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Geometry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &testgeometry, NULL);
	glCompileShader(geometry);

	glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(geometry, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &testfragment, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	// Check if shader has successfully compiled
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, geometry);
	glAttachShader(m_shaderProgram, fragment);
	glLinkProgram(m_shaderProgram);

	// Check for linking errors
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}



/// <summary>
/// Loads the specified shader, if shader already exists it returns a pointer to it instead of reloading the same shader
/// </summary>
/// <param name="vertexPath">Vertex shader file path</param>
/// <param name="fragmentPath">Fragment shader file path</param>
/// <returns>Pointer to the created shader</returns>
///
///

/// <summary>
/// Creates a shader object, using the specified filepath
/// If an object already exists with the same filepath, then return that instead of recreating the same object.
/// </summary>
/// <returns>Pointer to the created shader object</returns>
std::shared_ptr<Shader> ShaderManager::retrieveShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// Check if shader program is already loaded
	for (const auto& s : m_loadedShaders)
	{
		if (s->getFilePath(e_VertexShader) == vertexPath && s->getFilePath(e_FragmentShader) == fragmentPath)
		{
			return s;
		}
	}

	// Otherwise, create new shader and add it to vector
	std::cout << "SHADERMANAGER->" << vertexPath << " + " << fragmentPath << " program is being created" << std::endl;

	std::shared_ptr<Shader> s = std::make_shared<Shader>();
	s->setFilePath(vertexPath, fragmentPath);
	m_loadedShaders.push_back(s);

	return s;
}

/// <summary>
/// Creates a shader object, using the specified filepath
/// If an object already exists with the same filepath, then return that instead of recreating the same object.
/// </summary>
/// <returns>Pointer to the created shader object</returns>
std::shared_ptr<Shader> ShaderManager::retrieveShader(const GLchar* vertexPath, const GLchar* tessellationControlPath, const GLchar* tessellationEvaluationPath, const GLchar* fragmentPath)
{
	// Check if exact same shader program is already loaded
	for (const auto& s : m_loadedShaders)
	{
		if (s->getFilePath(e_VertexShader) == vertexPath &&
			s->getFilePath(e_TessellationControlShader) == tessellationControlPath &&
			s->getFilePath(e_TessellationEvaluationShader) == tessellationEvaluationPath &&
			s->getFilePath(e_FragmentShader) == fragmentPath)
		{
			return s;
		}
	}

	// Otherwise, create new texture and add it to vector
	std::cout << "SHADERMANAGER->" << vertexPath << " + " << tessellationControlPath << " + " << tessellationEvaluationPath << " + " << fragmentPath << " program is being created" << std::endl;

	std::shared_ptr<Shader> s = std::make_shared<Shader>();
	s->setFilePath(vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);
	m_loadedShaders.push_back(s);
	return s;
}

/// <summary>
/// Creates a shader object, using the specified filepath
/// If an object already exists with the same filepath, then return that instead of recreating the same object.
/// </summary>
/// <returns>Pointer to the created shader object</returns>
std::shared_ptr<Shader> ShaderManager::retrieveShader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath)
{
	// Check if exact same shader program is already loaded
	for (const auto s : m_loadedShaders)
	{
		if (s->getFilePath(e_VertexShader) == vertexPath &&
			s->getFilePath(e_GeometryShader) == geometryPath &&
			s->getFilePath(e_FragmentShader) == fragmentPath)
		{
			return s;
		}
	}

	// Otherwise, create new texture and add it to vector
	std::cout << "SHADERMANAGER->" << vertexPath << " + " << geometryPath << " + " << fragmentPath << " program is being created" << std::endl;

	std::shared_ptr<Shader> s = std::make_shared<Shader>();
	s->setFilePath(vertexPath, geometryPath, fragmentPath);
	m_loadedShaders.push_back(s);
	return s;
}

/// <summary>
/// Calls the .readShaderFromFile() function for each shader object created from ::retrieveShaderObject. This part reads the .txt data from the filepath and puts it into the object.
/// Part 1 of 2 for shader creation
/// </summary>
void ShaderManager::readShadersFromFile()
{
	for (auto& s : m_loadedShaders)
	{
		s->readShaderFromFile();
	}
}

/// <summary>
/// Calls the .loadShader function for each shader object. Uses the saved text data saved in the object to create the OpenGL shader program
/// Part 2 of 2 for shader creation
/// </summary>
void ShaderManager::createShaders()
{
	for (auto& s : m_loadedShaders)
	{
		s->loadShader();
	}
}

/// <summary>
/// Completely clears all saved shader objects
/// </summary>
void ShaderManager::clearShaders()
{
	m_loadedShaders.clear();
}
