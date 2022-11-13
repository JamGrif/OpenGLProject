#include "pch.h"
#include "Rendering/Shader.h"

#include "Rendering/OpenGLErrorCheck.h"

#include <glm\gtc\type_ptr.hpp>

#include <GL/glew.h>

#include <fstream>
#include <sstream>

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

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
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
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
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
		PRINT_WARN("SHADER-> Failed to open and read shaders {0} , {1} with error {2}", vertexPath, fragmentPath, e.what());
	}

	std::string vertexShaderCode = vertexCode.c_str();
	std::string fragmentShaderCode = fragmentCode.c_str();
	//m_shaderCode[e_VertexShader] = vertexCode.c_str();
	//m_shaderCode[e_FragmentShader] = fragmentCode.c_str();

	// All shaders contain both vertex and fragment shaders
	GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexShaderCode.c_str());
	GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode.c_str());

	m_shaderProgram = linkShaders(vertex, fragment);



	
}

Shader::~Shader()
{
	glCall(glDeleteProgram(m_shaderProgram));
}

void Shader::bindShader()
{
	glCall(glUseProgram(m_shaderProgram));
}

void Shader::unbindShader()
{
	glCall(glUseProgram(0));
}

/// <summary>
/// Sets 1 int uniform value
/// </summary>
/// <param name="name">Name of uniform in shader</param>
/// <param name="v0">Value of int</param>
void Shader::setUniform1i(const std::string& name, int v0)
{
	glCall(glUniform1i(getUniformLocation(name), v0));
}

/// <summary>
/// Sets 1 float uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of float</param>
void Shader::setUniform1f(const std::string& name, float v0)
{
	glCall(glUniform1f(getUniformLocation(name), v0));
}

/// <summary>
/// Sets 3 float uniform values
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the float 3</param>
void Shader::setUniform3f(const std::string& name, const glm::vec3& v0)
{
	glCall(glUniform3f(getUniformLocation(name), v0.x, v0.y, v0.z));
}

/// <summary>
/// Sets 4 float uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the float 4</param>
void Shader::setUniform4f(const std::string& name, const glm::vec4& v0)
{
	glCall(glUniform4f(getUniformLocation(name), v0.x, v0.y, v0.z, v0.w));
}

/// <summary>
/// Sets a 4x4 matrix uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the 4x4 matrix</param>
void Shader::setUniformMatrix4fv(const std::string& name, const glm::mat4& v0)
{
	glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0)));
}

/// <summary>
/// Sets a 3x3 matrix uniform value
/// </summary>
/// <param name="name">Name of the uniform in shader</param>
/// <param name="v0">Value of the 4x4 matrix</param>
void Shader::setUniformMatrix3fv(const std::string& name, const glm::mat3& v0)
{
	glCall(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0)));
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
		PRINT_WARN("SHADER-> Uniform location {0} does not exist", name);
	}
	m_locationCache[name] = location;

	return location;
}
