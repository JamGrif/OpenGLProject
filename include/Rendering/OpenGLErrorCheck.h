#pragma once

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// Wrap OpenGL error function with glCall to allow error checking in debug mode
#define ASSERT(x) if (!(x)) __debugbreak()
	#ifdef JG_DEBUG
		#define glCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
	#else
		#define glCall(x) x
	#endif
