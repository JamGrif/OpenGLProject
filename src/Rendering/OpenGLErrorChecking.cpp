#include "pch.h"
#include "Rendering/OpenGLErrorCheck.h"

#include "GL/glew.h" // glew.h must always be the first openGL header to be included (before glfw3.h)

/// <summary>
/// Loop through OpenGL error checking system until all errors are cleared
/// </summary>
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

/// <summary>
/// Print out OpenGL related errors from calling glCall()
/// </summary>
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		PRINT_ERROR("OpenGL-> gl function call failed with error {0}. Function: {1}, File: {2}:{3},", error, function, file, line);
		return false;
	}
	return true;
}
