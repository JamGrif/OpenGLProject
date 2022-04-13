#include "pch.h"
#include "Rendering/OpenGLErrorCheck.h"

#include <GL/glew.h> // glew.h must always be the first openGL header to be included (before glfw3.h)

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		PRINT_ERROR("OpenGL-> gl function call failed with error {0}. Function: {1}, File: {2}:{3},", error, function, file, line);
		//std::cout << "opengl error " << error << function << file << line << std::endl;
		return false;
	}
	return true;
}
