#include "pch.h"
#include "Rendering/Resource/Shader.h"

#include "Rendering/OpenGLErrorCheck.h"

#include "glm\gtc\type_ptr.hpp"

#include "GL/glew.h"

#include <fstream>
#include <sstream>

static constexpr unsigned int	NO_PROGRAM = 0;
static constexpr int			FAILED_TO_FIND = -1;

enum class SHADER_TYPE
{
	VertexShader	= GL_VERTEX_SHADER,
	FragmentShader	= GL_FRAGMENT_SHADER
};

/// <summary>
/// Compiles shader source code into OpenGL shader objects returning an internal index to it
/// </summary>
static GLuint CompileShader(SHADER_TYPE ShaderType, const GLchar* source)
{
	if (!source)
		return 0;

	// Create OpenGL shader and pass it shader source code
	GLuint newShader = glCreateShader(static_cast<int>(ShaderType));
	glCall(glShaderSource(newShader, 1, &source, NULL));

	glCall(glCompileShader(newShader));

	// Ensure compiling success
	GLint compilingSuccess;
	glCall(glGetShaderiv(newShader, GL_COMPILE_STATUS, &compilingSuccess));
	if (!compilingSuccess)
	{
		constexpr int errorLength = 512;
		GLchar infoLog[errorLength];
		glCall(glGetShaderInfoLog(newShader, errorLength, NULL, infoLog));

		glCall(glDeleteShader(newShader));

		PRINT_WARN("SHADER-> Failed to compile shader - {0}", infoLog);
		return 0;
	}

	return newShader;
}

/// <summary>
/// Link OpenGL shaders, via their internal index, into a complete OpenGL program
/// </summary>
static GLuint LinkShaders(GLuint firstShader, GLuint secondShader)
{
	if (!firstShader || !secondShader)
		return 0;

	// Create OpenGL program and attach the shaders to the program (only attaches ones that are valid)
	GLuint newShaderProgram = glCreateProgram();

	// Attach valid shaders to program
	glCall(glAttachShader(newShaderProgram, firstShader));
	glCall(glAttachShader(newShaderProgram, secondShader));

	glCall(glLinkProgram(newShaderProgram));

	// Delete shaders as no longer required
	glCall(glDeleteShader(firstShader)); 
	glCall(glDeleteShader(secondShader)); 

	// Ensure linking success
	GLint linkingSuccess;
	glCall(glGetProgramiv(newShaderProgram, GL_LINK_STATUS, &linkingSuccess));
	if (!linkingSuccess)
	{
		constexpr int errorLength = 512;
		GLchar infoLog[errorLength];
		glCall(glGetProgramInfoLog(newShaderProgram, errorLength, NULL, infoLog));

		glCall(glDeleteProgram(newShaderProgram));

		PRINT_WARN("SHADER-> Failed to link shader program - {0}", infoLog);
		return 0;
	}

	return newShaderProgram;
}

Shader::Shader()
	:IResource()
{
}

Shader::~Shader()
{
	// Unbind and delete the shader program
	glCall(glUseProgram(NO_PROGRAM));
	glCall(glDeleteProgram(m_OpenGLResourceID));
}

/// <summary>
/// 1 / 2 of shader creation
/// Parse the .glsl shader file at vertexPath and fragmentPath
/// </summary>
void Shader::Parse(const std::string& vertexPath, const std::string& fragmentPath)
{
	try
	{
		// Open files
		std::ifstream vShaderFile(vertexPath);
		std::ifstream fShaderFile(fragmentPath);

		// Ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		// Read file's buffer contents into streams
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vertexShaderCode = vShaderStream.str();
		fragmentShaderCode = fShaderStream.str();

		// Close files
		vShaderFile.close();
		fShaderFile.close();
	}
	catch (const std::ifstream::failure e)
	{
		PRINT_WARN("SHADER-> Failed to open and read shaders {0} , {1} with error {2}", vertexPath, fragmentPath, e.what());
	}
}

/// <summary>
/// 2 / 2 of shader creation
/// Use parsed shader data to create shader programs
/// </summary>
void Shader::Create()
{
	// Compile the parsed shader file
	GLuint vertex = CompileShader(SHADER_TYPE::VertexShader, vertexShaderCode.c_str());
	GLuint fragment = CompileShader(SHADER_TYPE::FragmentShader, fragmentShaderCode.c_str());

	// Attach the compiled shaders to the shader program
	m_OpenGLResourceID = LinkShaders(vertex, fragment);

	m_bIsCreated = true;
}

/// <summary>
/// Bind the shader program to the OpenGL context
/// </summary>
void Shader::Bind()
{
	glCall(glUseProgram(m_OpenGLResourceID));
}

/// <summary>
/// Unbind the shader program to the OpenGL context
/// </summary>
void Shader::Unbind()
{
	glCall(glUseProgram(NO_PROGRAM));
}

/// <summary>
/// Reset the OpenGL stuff for the shader - avoids having to re-parse the same file
/// </summary>
void Shader::Reset()
{
	// Delete internal OpenGL program
	glCall(glDeleteProgram(m_OpenGLResourceID));

	// Create a new program with parsed shader source code
	Create();

	// Clear the location cache
	m_uniformLocationCache.clear();
}

/// <summary>
/// Set an int uniform value at uniformName
/// </summary>
void Shader::SetUniform(const std::string& uniformName, int value)
{
	glCall(glUniform1i(GetUniformLocation(uniformName), value));
}

/// <summary>
/// Set a float uniform value at uniformName
/// </summary>
void Shader::SetUniform(const std::string& uniformName, float value)
{
	glCall(glUniform1f(GetUniformLocation(uniformName), value));
}

/// <summary>
/// Set a vector3 uniform value at uniformName
/// </summary>
void Shader::SetUniform(const std::string& uniformName, const Vector3D& value)
{
	glCall(glUniform3f(GetUniformLocation(uniformName), value.GetX(), value.GetY(), value.GetZ()));
}

/// <summary>
/// Set a vector4 uniform value at uniformName
/// </summary>
void Shader::SetUniform(const std::string& uniformName, const glm::vec4& value)
{
	glCall(glUniform4f(GetUniformLocation(uniformName), value.x, value.y, value.z, value.w));
}

/// <summary>
/// Set 4x4 matrix uniform value at uniformName
/// </summary>
void Shader::SetUniform(const std::string& uniformName, const glm::mat4& value)
{
	glCall(glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value)));
}

/// <summary>
/// Set the 3x3 matrix uniform value at uniformName
/// </summary>
void Shader::SetUniform(const std::string& uniformName, const glm::mat3& value)
{
	glCall(glUniformMatrix3fv(GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value)));
}

/// <summary>
/// Find the location of uniformName in shader file, storing it if first time
/// </summary>
int Shader::GetUniformLocation(const std::string& uniformName)
{
	// Get uniform location of uniformName
	if (m_uniformLocationCache.find(uniformName) != m_uniformLocationCache.end())
		return m_uniformLocationCache[uniformName];
	
	// If uniformName doesn't currently exist, find and store its location
	int uniformLocation = glGetUniformLocation(m_OpenGLResourceID, uniformName.c_str());

	if (uniformLocation == FAILED_TO_FIND)
		PRINT_WARN("SHADER-> Uniform location {0} does not exist", uniformName);
	else
		m_uniformLocationCache[uniformName] = uniformLocation;
	
	return uniformLocation;
}
