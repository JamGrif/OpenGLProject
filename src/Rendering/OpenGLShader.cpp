#include "pch.h"
#include "rendering/OpenGLShader.h"

#include "Rendering/OpenGLErrorCheck.h"

#include <glm\gtc\type_ptr.hpp>

#include <fstream>
#include <sstream>

#include <GL/glew.h>

std::vector<std::shared_ptr<OpenGLShader>> ShaderManager::m_loadedShaders;

/// <summary>
/// Specifies what type of shader the source code is
/// </summary>
enum shaderFilePath
{
	e_VertexShader					= 0,
	e_FragmentShader				= 1,
	e_TessellationControlShader		= 2,
	e_TessellationEvaluationShader	= 3,
	e_GeometryShader				= 4
};

/// <summary>
/// Utility function to compile the given shader code into a shader and returns a OpenGL ID to it
/// </summary>
/// <param name="shaderType">Only: GL_VERTEX_SHADER / GL_FRAGMENT_SHADER / GL_GEOMETRY_SHADER / GL_TESS_CONTROL_SHADER / GL_TESS_EVALUATION_SHADER</param>
/// <param name="source">Shader source code</param>
/// <returns>OpenGL ID to shader</returns>
static const GLuint compileShader(int GLShaderType, const GLchar* source)
{
	// Create OpenGL shader, give it the source code and compile it
	GLuint tempID = glCreateShader(GLShaderType);
	glCall(glShaderSource(tempID, 1, &source, NULL));

	glCall(glCompileShader(tempID));

	// Check for compiling errors and print any errors
	GLint success;
	glCall(glGetShaderiv(tempID, GL_COMPILE_STATUS, &success));

	if (!success)
	{
		constexpr int errorLength = 512;
		GLchar infoLog[errorLength];
		glCall(glGetShaderInfoLog(tempID, errorLength, NULL, infoLog));

		PRINT_WARN("SHADER-> Failed to compile shader - {0}", infoLog);
	}

	return tempID;
}

/// <summary>
/// Utility function that receives compiled shader IDs and uses them to create and return a shader program
/// Can receive a range of 1 - 4 shaders to combine into a program
/// </summary>
/// <returns>OpenGL ID to shader program</returns>
static const GLuint linkShaders(GLuint shader1 = 0, GLuint shader2 = 0, GLuint shader3 = 0, GLuint shader4 = 0)
{
	// Create OpenGL program and attach the shaders to the program (only attaches ones that are valid)
	GLuint tempProgram;
	tempProgram = glCreateProgram();

	// Attach valid shaders to program
	if (shader1) { glCall(glAttachShader(tempProgram, shader1)); }
	if (shader2) { glCall(glAttachShader(tempProgram, shader2)); }
	if (shader3) { glCall(glAttachShader(tempProgram, shader3)); }
	if (shader4) { glCall(glAttachShader(tempProgram, shader4)); }
		
	glCall(glLinkProgram(tempProgram));

	// Check for linking errors and print any errors
	GLint success;
	glCall(glGetProgramiv(tempProgram, GL_LINK_STATUS, &success));

	if (!success)
	{
		constexpr int errorLength = 512;
		GLchar infoLog[errorLength];
		glCall(glGetProgramInfoLog(tempProgram, errorLength, NULL, infoLog));

		PRINT_WARN("SHADER-> Failed to link shader program - {0}", infoLog);
	}

	// Delete valid shaders as they're linked into the program and no longer necessary
	if (shader1) { glCall(glDeleteShader(shader1)); }
	if (shader2) { glCall(glDeleteShader(shader2)); }
	if (shader3) { glCall(glDeleteShader(shader3)); }
	if (shader4) { glCall(glDeleteShader(shader4)); }
		
	return tempProgram;
}

OpenGLShader::OpenGLShader()
	:m_shaderProgram(0), m_shaderType(shaderProgramType::e_NoProgramType)
{
}

OpenGLShader::~OpenGLShader()
{
	glCall(glDeleteProgram(m_shaderProgram));
}

/// <summary>
/// Binds the shader
/// </summary>
void OpenGLShader::Bind() const
{
	glCall(glUseProgram(m_shaderProgram));
}

/// <summary>
/// Unbinds the shader
/// </summary>
void OpenGLShader::Unbind() const
{
	glCall(glUseProgram(0));
}

/// <summary>
/// Sets 1 int uniform value
/// </summary>
/// <param name="name">Name of uniform in shader</param>
/// <param name="v0">Value of int</param>
void OpenGLShader::setUniform1i(const std::string& name, int v0)
{
	glCall(glUniform1i(getUniformLocation(name), v0));
}

/// <summary>
/// Sets 1 float uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of float</param>
void OpenGLShader::setUniform1f(const std::string& name, float v0)
{
	glCall(glUniform1f(getUniformLocation(name), v0));
}

/// <summary>
/// Sets 3 float uniform values
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the float 3</param>
void OpenGLShader::setUniform3f(const std::string& name, const glm::vec3& v0)
{
	glCall(glUniform3f(getUniformLocation(name), v0.x, v0.y, v0.z));
}

/// <summary>
/// Sets 4 float uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the float 4</param>
void OpenGLShader::setUniform4f(const std::string& name, const glm::vec4& v0)
{
	glCall(glUniform4f(getUniformLocation(name), v0.x, v0.y, v0.z, v0.w));
}

/// <summary>
/// Sets a 4x4 matrix uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the 4x4 matrix</param>
void OpenGLShader::setUniformMatrix4fv(const std::string& name, const glm::mat4& v0)
{
	glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0)));
}

/// <summary>
/// Sets a 3x3 matrix uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the 4x4 matrix</param>
void OpenGLShader::setUniformMatrix3fv(const std::string& name, const glm::mat3& v0)
{
	glCall(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0)));
}

/// <summary>
/// Uses the saved filepath (from .setFilePath()) to read the shader textfile into the object.
/// Must be called BEFORE .loadShader()
/// </summary>
void OpenGLShader::readSourceCodeFromFile()
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
		PRINT_WARN("SHADER-> Failed to open and read shaders {0} , {1} with error {2}", m_shaderFilePaths[e_VertexShader], m_shaderFilePaths[e_FragmentShader], e.what());
	}

	m_shaderCode[e_VertexShader] = vertexCode.c_str();
	m_shaderCode[e_FragmentShader] = fragmentCode.c_str();

	if (m_shaderType == shaderProgramType::e_NormalProgramType)
	{
		return;
	}

	// If a tessellation shader, also read and save from the tessellation shader text files
	if (m_shaderType == shaderProgramType::e_TessellationProgramType)
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
			PRINT_WARN("SHADER-> Failed to open and read shaders {0}, {1} with error {2}", m_shaderFilePaths[e_TessellationControlShader], m_shaderFilePaths[e_TessellationEvaluationShader], e.what());
		}
		m_shaderCode[e_TessellationControlShader] = tcCode.c_str();
		m_shaderCode[e_TessellationEvaluationShader] = teCode.c_str();

		return;
	}

	// If a geometry shader, also read and save from the tessellation shader text files
	if (m_shaderType == shaderProgramType::e_GeometryProgramType)
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
			PRINT_WARN("SHADER-> Failed to open and read shaders {0} with error {2}", m_shaderFilePaths[e_GeometryShader], e.what());
		}
		m_shaderCode[e_GeometryShader] = geoCode.c_str();
	}
}

/// <summary>
/// Compiles the shader source, links it and creates the OpenGL shader program
/// Must be called after .readFromShaderFile() loadShader
/// </summary>
void OpenGLShader::createShaderProgram()
{
	// All shaders contain both vertex and fragment shaders
	GLuint vertex = compileShader(GL_VERTEX_SHADER, m_shaderCode[e_VertexShader].c_str());
	GLuint fragment = compileShader(GL_FRAGMENT_SHADER, m_shaderCode[e_FragmentShader].c_str());

	if (m_shaderType == shaderProgramType::e_NormalProgramType) // vertex / fragment
	{
		m_shaderProgram = linkShaders(vertex, fragment);
	}
	else if (m_shaderType == shaderProgramType::e_TessellationProgramType) // vertex / fragment / tControl / tEvaluation
	{
		GLuint tcontrol = compileShader(GL_TESS_CONTROL_SHADER, m_shaderCode[e_TessellationControlShader].c_str());
		GLuint tevaluaction = compileShader(GL_TESS_EVALUATION_SHADER, m_shaderCode[e_TessellationEvaluationShader].c_str());

		m_shaderProgram = linkShaders(vertex, fragment, tcontrol, tevaluaction);
	}
	else if (m_shaderType == shaderProgramType::e_GeometryProgramType) // vertex / fragment / geometry
	{
		GLuint geometry = compileShader(GL_GEOMETRY_SHADER, m_shaderCode[e_GeometryShader].c_str());

		m_shaderProgram = linkShaders(vertex, fragment, geometry);
	}
}

/// <summary>
/// NORMAL VERTEX/FRAGMENT SHADER ONLY - Sets the shader type to be a normal shader and saves the file paths for the specific shader files 
/// </summary>
void OpenGLShader::setFilePath(const std::string& vertexPath, const std::string& fragmentPath)
{
	m_shaderType = shaderProgramType::e_NormalProgramType;

	m_shaderFilePaths[e_VertexShader] = vertexPath;
	m_shaderFilePaths[e_FragmentShader] = fragmentPath;
}

/// <summary>
/// TESSELLATION SHADER ONLY - Sets the shader type to be a tessellation shader and saves the file paths for the specific shader files 
/// </summary>
void OpenGLShader::setFilePath(const std::string& vertexPath, const std::string& tessellationControlPath, const std::string& tessellationEvaluationPath, const std::string& fragmentPath)
{
	m_shaderType = shaderProgramType::e_TessellationProgramType;

	m_shaderFilePaths[e_VertexShader] = vertexPath;
	m_shaderFilePaths[e_TessellationControlShader] = tessellationControlPath;
	m_shaderFilePaths[e_TessellationEvaluationShader] = tessellationEvaluationPath;
	m_shaderFilePaths[e_FragmentShader] = fragmentPath;
}

/// <summary>
/// GEOMETRY SHADER ONLY - Sets the shader type to be a geometry shader and saves the file paths for the specific shader files 
/// </summary>
void OpenGLShader::setFilePath(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath)
{
	m_shaderType = shaderProgramType::e_GeometryProgramType;

	m_shaderFilePaths[e_VertexShader] = vertexPath;
	m_shaderFilePaths[e_GeometryShader] = geometryPath;
	m_shaderFilePaths[e_FragmentShader] = fragmentPath;
}


/// <summary>
/// Returns the filepath to a specific shader file
/// </summary>
const std::string& OpenGLShader::getFilePath(int filePath) const
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

	default:
		return m_shaderFilePaths[e_VertexShader]; // return vertex shader filepath by default

	}
}

/// <summary>
/// Returns if the shader is a normal, tessellation or geometry shader
/// </summary>
/// <returns></returns>
const shaderProgramType OpenGLShader::getShaderType() const
{
	return m_shaderType;
}

/// <summary>
/// Cache system that only finds the location of a uniform once and then stores it location.
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <returns>Location of the uniform value</returns>
const int OpenGLShader::getUniformLocation(const std::string& name)
{
	if (m_locationCache.find(name) != m_locationCache.end())
	{
		return m_locationCache[name];
	}

	int location = glGetUniformLocation(m_shaderProgram, name.c_str());
	if (location == -1)
	{
		PRINT_WARN("SHADER-> Uniform location {0} does not exist", name);
	}
	m_locationCache[name] = location;

	return location;
}


/// <summary>
/// Creates a shader object, using the specified filepath
/// If an object already exists with the same filepath, then return that instead of recreating the same object.
/// </summary>
/// <returns>Pointer to the created shader object</returns>
std::shared_ptr<OpenGLShader> ShaderManager::retrieveShader(const char* vertexPath, const char* fragmentPath)
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
	//PRINT_TRACE("SHADERMANAGER-> {0} + {1} program is being created", vertexPath, fragmentPath);

	std::shared_ptr<OpenGLShader> s = std::make_shared<OpenGLShader>();
	s->setFilePath(vertexPath, fragmentPath);
	m_loadedShaders.push_back(s);

	return s;
}

/// <summary>
/// Creates a shader object, using the specified filepath
/// If an object already exists with the same filepath, then return that instead of recreating the same object.
/// </summary>
/// <returns>Pointer to the created shader object</returns>
std::shared_ptr<OpenGLShader> ShaderManager::retrieveShader(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath)
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
	//PRINT_TRACE("SHADERMANAGER-> {0} + {1} + {2} + {3} program is being created", vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);

	std::shared_ptr<OpenGLShader> s = std::make_shared<OpenGLShader>();
	s->setFilePath(vertexPath, tessellationControlPath, tessellationEvaluationPath, fragmentPath);
	m_loadedShaders.push_back(s);
	return s;
}

/// <summary>
/// Creates a shader object, using the specified filepath
/// If an object already exists with the same filepath, then return that instead of recreating the same object.
/// </summary>
/// <returns>Pointer to the created shader object</returns>
std::shared_ptr<OpenGLShader> ShaderManager::retrieveShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
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

	// Otherwise, create new shader and add it to vector
	//PRINT_TRACE("SHADERMANAGER-> {0} + {1} + {2} program is being created", vertexPath, geometryPath, fragmentPath);

	std::shared_ptr<OpenGLShader> s = std::make_shared<OpenGLShader>();
	s->setFilePath(vertexPath, geometryPath, fragmentPath);
	m_loadedShaders.push_back(s);
	return s;
}

/// <summary>
/// Calls the .readSourceCodeFromFile() function for each shader object created from ::retrieveShaderObject. This part reads the .txt data from the filepath and puts it into the object.
/// Part 1 of 2 for shader creation
/// </summary>
void ShaderManager::readShadersFromFile()
{
	for (auto& s : m_loadedShaders)
	{
		s->readSourceCodeFromFile();
	}
}

/// <summary>
/// Calls the .createShaderProgram function for each shader object. Uses the saved text data saved in the object to create the OpenGL shader program
/// Part 2 of 2 for shader creation
/// </summary>
void ShaderManager::createShaders()
{
	for (auto& s : m_loadedShaders)
	{
		s->createShaderProgram();
	}
}

/// <summary>
/// Completely clears all saved shader objects
/// </summary>
void ShaderManager::clearShaders()
{
	m_loadedShaders.clear();
}
