#include "pch.h"
#include "Rendering/Shader.h"

#include "Rendering/OpenGLErrorCheck.h"

#include <glm\gtc\type_ptr.hpp>

#include <GL/glew.h>

#include <fstream>
#include <sstream>

static constexpr unsigned int	NO_PROGRAM = 0;
static constexpr int			FAILED_TO_FIND = -1;

enum class SHADER_TYPE
{
	VertexShader = GL_VERTEX_SHADER,
	FragmentShader = GL_FRAGMENT_SHADER
};

/// <summary>
/// Utility function to compile the given shader code into a shader and returns a OpenGL ID to it
/// </summary>
/// <param name="shaderType">Only: GL_VERTEX_SHADER / GL_FRAGMENT_SHADER / GL_GEOMETRY_SHADER / GL_TESS_CONTROL_SHADER / GL_TESS_EVALUATION_SHADER</param>
/// <param name="source">Shader source code</param>
/// <returns>OpenGL ID to shader</returns>
static const GLuint CompileShader(SHADER_TYPE ShaderType, const GLchar* source)
{
	// Create OpenGL shader, give it the source code and compile it
	GLuint tempID = glCreateShader(static_cast<int>(ShaderType));
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
static const GLuint LinkShaders(GLuint firstShader, GLuint secondShader)
{
	// Create OpenGL program and attach the shaders to the program (only attaches ones that are valid)
	GLuint newShaderProgram;
	newShaderProgram = glCreateProgram();

	// Attach valid shaders to program
	if (firstShader) { glCall(glAttachShader(newShaderProgram, firstShader)); }
	if (secondShader) { glCall(glAttachShader(newShaderProgram, secondShader)); }

	glCall(glLinkProgram(newShaderProgram));

	// Check for linking errors and print any errors
	GLint success;
	glCall(glGetProgramiv(newShaderProgram, GL_LINK_STATUS, &success));

	if (!success)
	{
		constexpr int errorLength = 512;
		GLchar infoLog[errorLength];
		glCall(glGetProgramInfoLog(newShaderProgram, errorLength, NULL, infoLog));

		PRINT_WARN("SHADER-> Failed to link shader program - {0}", infoLog);
	}

	// Delete valid shaders as they're linked into the program and no longer necessary
	if (firstShader) { glCall(glDeleteShader(firstShader)); }
	if (secondShader) { glCall(glDeleteShader(secondShader)); }

	return newShaderProgram;
}

Shader::Shader()
	:m_shaderProgram(NO_PROGRAM), m_bCreated(false)
{
}

Shader::~Shader()
{
	glCall(glDeleteProgram(m_shaderProgram));
}

void Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath)
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

		// Convert stream into string
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

void Shader::CreateShader()
{
	// Compile the parsed shader file
	GLuint vertex = CompileShader(SHADER_TYPE::VertexShader, vertexShaderCode.c_str());
	GLuint fragment = CompileShader(SHADER_TYPE::FragmentShader, fragmentShaderCode.c_str());

	// Attach the compiled shaders to the shader program
	m_shaderProgram = LinkShaders(vertex, fragment);

	vertexShaderCode.clear();
	fragmentShaderCode.clear();

	m_bCreated = true;
}

/// <summary>
/// Bind the shader program to the OpenGL context
/// </summary>
void Shader::BindShader()
{
	glCall(glUseProgram(m_shaderProgram));
}

/// <summary>
/// Unbind the shader program to the OpenGL context
/// </summary>
void Shader::UnbindShader()
{
	glCall(glUseProgram(NO_PROGRAM));
}

/// <summary>
/// Set a float uniform value at uniformName
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
void Shader::SetUniform(const std::string& uniformName, const glm::vec3& value)
{
	glCall(glUniform3f(GetUniformLocation(uniformName), value.x, value.y, value.z));
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
	{
		return m_uniformLocationCache[uniformName];
	}

	// If uniformName doesn't currently exist, find and store its location
	int uniformLocation = glGetUniformLocation(m_shaderProgram, uniformName.c_str());

	if (uniformLocation == FAILED_TO_FIND)
	{
		PRINT_WARN("SHADER-> Uniform location {0} does not exist", uniformName);
	}
	else
	{
		m_uniformLocationCache[uniformName] = uniformLocation;
	}

	return uniformLocation;
}
